#include "widget.h"
#include "ui_widget.h"
#include"weathertool.h"
#include <QMessageBox>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);
    //设置窗口固定大小
    setFixedSize(QSize(800, 450));
    //构建菜单
    mExitMenu = new QMenu(this);
    mExitAction = new QAction(this);
    //设置菜单项的名字和图标
    mExitAction->setText("退出");
    mExitAction->setIcon(QIcon(":/res/close.png"));
    //添加到菜单上
    mExitMenu->addAction(mExitAction);
    //连接槽函数
    connect(mExitAction, &QAction::triggered, this, [=]{
        qApp->exit();
    });


    //请求完毕之后读取数据，并对数据进行处理
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &Widget::onReplied);

    connect(ui->btnSearch, &QPushButton::clicked, this, [=](){
        QString cityName=ui->leCity_2->text();
        qDebug()<<cityName;
        getWeatherInfo(cityName);
    });


}


//读取请求数据
void Widget::onReplied(QNetworkReply *reply)
{
    //状态码
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //判断是否请求成功
    if (reply->error() != QNetworkReply::NoError || statusCode != 200) {
        QMessageBox::warning(this, "天气", reply->errorString(), QMessageBox::Ok);
    }else{
        QByteArray requestData = reply->readAll();
        //qDebug() << requestData.data();
        //QJsonDocument doc = QJsonDocument::fromJson(requestData);
        parseJson(requestData);//解析json数据,创建2个类,ToDay和Day(数组)
    }
    //释放内存
    reply->deleteLater();
}


//获取天气信息
void Widget::getWeatherInfo(QString cityName)
{
    QString cityCode = weatherTool::getCityCode(cityName);
    //检查返回的是否为空值
    if (cityCode.isEmpty()) {
        QMessageBox::information(this, "提示", "请检查输入的是否正确!", QMessageBox::Ok);
        return;
    }
    QUrl url = QUrl("http://t.weather.itboy.net/api/weather/city/" + cityCode);
    mNetworkAccessManager->get(QNetworkRequest(url));
}


void Widget::parseWeatherData(QByteArray data)
{
    QJsonParseError err;
    //创建QJsonDocument
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        qDebug("%s(%d): %s", __FUNCTION__, __LINE__, err.errorString().toLatin1().data());
        return;
    }
    //Json对象,包括了请求的全部数据
    QJsonObject rootObj = doc.object();
    //qDebug() << rootObj;
    QJsonObject dataObj = rootObj.value("data").toObject();
    QJsonObject yesterdayObj = dataObj.value("yesterday").toObject();
    //Json数组,未来几天的数据
    QJsonArray forecastArray = dataObj.value("forecast").toArray();
    //1.解析日期和城市
    mToday.date = rootObj.value("date").toString();
    mToday.city = rootObj.value("cityInfo").toObject().value("city").toString();
    //qDebug() << mToday.date << mToday.city;
    //2.解析yesterday
    mDay[0].date = yesterdayObj.value("date").toString();//日期
    mDay[0].aqi = yesterdayObj.value("aqi").toDouble();//空气质量,toDouble包括整数型
    mDay[0].type = yesterdayObj.value("type").toString();//天气类型
    mDay[0].week = yesterdayObj.value("week").toString();//星期
    mDay[0].ymd = yesterdayObj.value("ymd").toString();//完整年月日
    //解析最低温度和最高温度
    /*
    QString str = yesterdayObj.value("low").toString();//最低温度
    str = str.split(" ").at(1);//以空格为分割符,去除前面的文字,取出第二个X℃,
    str = str.left(str.length() - 1);//去除后面的摄氏度符号
    mDay[0].low = str.toInt();//传入最低温度
    str.clear();//清除内容
    str = yesterdayObj.value("high").toString();//最高温度
    str = str.split(" ").at(1);//以空格为分割符,去除前面的文字,取出第二个X℃,
    str = str.left(str.length() - 1);//去除后面的摄氏度符号
    mDay[0].high = str.toInt();//传入最高温度
    */
    //简化版
    mDay[0].low = parseString(yesterdayObj.value("low").toString());//最低温度
    mDay[0].high = parseString(yesterdayObj.value("high").toString());//最高温度
    mDay[0].fx = yesterdayObj.value("fx").toString();//风向
    mDay[0].fl = yesterdayObj.value("fl").toString();//风力
    //qDebug() << mDay[0].date << mDay[0].aqi << mDay[0].type << mDay[0].week << mDay[0].low << mDay[0].high << mDay[0].fx << mDay[0].fl;
    //3.解析forecast中的6天数据
    //qDebug() << forecastArray.size();
    for (int i = 0; i < 5; ++i) {
        QJsonObject forecastData = forecastArray[i].toObject();
        //跳过0,因为0已经赋值给昨天的数据了
        mDay[i + 1].date = forecastData.value("date").toString();//日期
        mDay[i + 1].aqi = forecastData.value("aqi").toDouble();//空气质量,toDouble包括整数型
        mDay[i + 1].type = forecastData.value("type").toString();//天气类型
        mDay[i + 1].week = forecastData.value("week").toString();//星期
        mDay[i + 1].low = parseString(forecastData.value("low").toString());//最低温度
        mDay[i + 1].high = parseString(forecastData.value("high").toString());//最高温度
        mDay[i + 1].fx = forecastData.value("fx").toString();//风向
        mDay[i + 1].fl = forecastData.value("fl").toString();//风力
        mDay[i + 1].ymd = forecastData.value("ymd").toString();//完整年月日
        //ymd分割成MM/dd
        //QDateTime::fromString(forecastData.value("ymd").toString(), "yyyy-MM-dd").toString("MM/dd");//日期
        //qDebug() << i << mDay[i + 1].date << mDay[i + 1].aqi << mDay[i + 1].type << mDay[i + 1].week << mDay[i + 1].low << mDay[i + 1].high << mDay[i + 1].fx << mDay[i + 1].fl;
        //qDebug() << i << mDay[i].date;
        //qDebug() << mDay[i + 1].ymd;
    }
    //4.解析今天的数据
    mToday.shidu = dataObj.value("shidu").toString();
    mToday.quality = dataObj.value("quality").toString();
    mToday.ganmao = dataObj.value("ganmao").toString();
    mToday.pm25 = dataObj.value("pm25").toDouble();
    mToday.wendu = dataObj.value("wendu").toString().toInt();
    //qDebug() << dataObj;
    //qDebug() << mToday.shidu << mToday.quality << mToday.ganmao << mToday.pm25 << mToday.wendu;
    //注意:forecast中的第2个元素也是今天的数据
    mToday.type = mDay[1].type;
    mToday.low = mDay[1].low;
    mToday.high = mDay[1].high;
    mToday.fx = mDay[1].fx;
    mToday.fl = mDay[1].fl;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    //获取偏移
    mOffset = event->globalPosition().toPoint() - this->pos();
    //this->pos()左上角的位置
    //globalPosition.toPoint() 鼠标点击的位置相对于左上角的位置
    //qDebug() << mOffset;
}


void Widget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - mOffset);
}


//重写父类的虚函数,因为父类的实现默认忽略点击事件
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    //需要Menu和Action
    //弹出右键菜单
    mExitMenu->exec(QCursor::pos());
    //事件已经处理,不需要向上传
    event->accept();
}


Widget::~Widget()
{
    delete ui;
}
