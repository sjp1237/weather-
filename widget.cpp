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

    ComponentInit();
    mNetworkAccessManager = new QNetworkAccessManager(this);
    //请求完毕之后读取数据，并对数据进行处理
   connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, Widget::onReplied);


    connect(ui->btnSearch, &QPushButton::clicked, this, [=](){
        QString cityName=ui->leCity_2->text();
        qDebug()<<cityName;
        getWeatherInfo(cityName);
    });


}

void Widget::ComponentInit(){
    mDateList << ui->lblDate0 << ui->lblDate1 << ui->lblDate2 << ui->lblDate3 << ui->lblDate4 << ui->lblDate5;
    mWeekList << ui->lblWeek0 << ui->lblWeek1 << ui->lblWeek2 << ui->lblWeek3 << ui->lblWeek4 << ui->lblWeek5;
    mQulityList << ui->lblQuality0 << ui->lblQuality1 << ui->lblQuality2 << ui->lblQuality3 << ui->lblQuality4 << ui->lblQuality5;
    mTypeList << ui->lblType0 << ui->lblType1 << ui->lblType2 << ui->lblType3 << ui->lblType4 << ui->lblType5;
    mTypeIconList << ui->lblTypeIcon0 << ui->lblTypeIcon1 << ui->lblTypeIcon2 << ui->lblTypeIcon3 << ui->lblTypeIcon4 << ui->lblTypeIcon5;
    mFxList << ui->lblFx0 << ui->lblFx1 << ui->lblFx2 << ui->lblFx3 << ui->lblFx4 << ui->lblFx5;
    mFlList << ui->lblFl0 << ui->lblFl1 << ui->lblFl2 << ui->lblFl3 << ui->lblFl4 << ui->lblFl5;
    //安装事件过滤器,用于拦截框架发送的事件QEvent::Point事件(标签调用update函数后会自动发送的),拦截后就可以区分是哪一个,进行绘制曲线
    ui->lblHighCurve->installEventFilter(this);
    ui->lblLowCurve->installEventFilter(this);
    //根据keys,设置icon的路径
    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu. png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/DaYu.png");
    mTypeMap.insert("冻雨",":/res/type/DongYu.png");
    mTypeMap.insert("多云",":/res/type/DuoYun.png");
    mTypeMap.insert("浮沉",":/res/type/FuChen.png");
    mTypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing.png");
    mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("undefined",":/res/type/undefined.png");
    mTypeMap.insert("雾",":/res/type/Wu.png");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/res/type/XiaoYu.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/res/type/YangSha.png");
    mTypeMap.insert("阴",":/res/type/Yin.png");
    mTypeMap.insert("雨",":/res/type/Yu.png");
    mTypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    mTypeMap.insert("阵雨",":/res/type/ZhenYu.png");
    mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
    mTypeMap.insert("中雨",":/res/type/ZhongYu.png");
}

void Widget::updateUi()
{
    QString changeStr = QDateTime::fromString(mToday.date, "yyyyMMdd").toString("yyyy/MM/dd");
    ui->lbDate_2->setText(changeStr + " " + mDay[1].week);
    ui->lbCity->setText(mToday.city);
    ui->lbPm25->setText(QString::number(mToday.pm25));
    ui->lbLowHigh->setText(QString::number(mToday.minimumTemperature) + "℃" + "~" + QString::number(mToday.maximumTemperature) + "℃" );
    ui->lbGaoMao->setText(mToday.ganmao);
    ui->lbShiDu->setText(mToday.shidu);
    ui->lblQuality ->setText(QString::number(mDay[1].aqi));
    ui->lbWindFx->setText(mToday.fx);
    ui->lbWindFl->setText(mToday.fl);
    ui->lbTemp->setText(QString::number(mToday.wendu) + "℃");
    //更新icon
    ui->lbTypeIcon->setPixmap(mTypeMap[mToday.type]);
    //qDebug() << mToday.type;
    //更新未来几天的数据
    for (int i = 0; i < 6; ++i) {
        //星期
        mWeekList[i]->setText("周" + mDay[i].week.right(1));
        mWeekList[0]->setText("昨天");
        mWeekList[1]->setText("今天");
        mWeekList[2]->setText("明天");
        //日期
        QStringList ymdList = mDay[i].ymd.split("-");
        mDateList[i]->setText(ymdList.at(1) + "-" + ymdList.at(2));
        //更新空气质量
        switch (checkedQulity(mDay[i].aqi)) {
        case 1:
            mQulityList[i]->setText("优");
            mQulityList[i]->setStyleSheet("background-color: rgb(121,184,0);");
            break;
        case 2:
            mQulityList[i]->setText("良");
            mQulityList[i]->setStyleSheet("background-color: rgb(255,187,23);");
            break;
        case 3:
            mQulityList[i]->setText("轻度");
            mQulityList[i]->setStyleSheet("background-color: rgb(255,87,97);");
            break;
        case 4:
            mQulityList[i]->setText("中度");
            mQulityList[i]->setStyleSheet("background-color: rgb(235,17,27);");
            break;
        case 5:
            mQulityList[i]->setText("重度");
            mQulityList[i]->setStyleSheet("background-color: rgb(170,0,0);");
            break;
        case 6:
            mQulityList[i]->setText("严重");
            mQulityList[i]->setStyleSheet("background-color: rgb(110,0,0);");
            break;
        }
        //风向,风力
        mFxList[i]->setText(mDay[i].fx);
        mFlList[i]->setText(mDay[i].fl);
        //天气类型和天气类型的Icon
        mTypeIconList[i]->setPixmap(mTypeMap[mDay[i].type]);
        mTypeList[i]->setText(mDay[i].type);
    }
}


//检查空气质量
int Widget::checkedQulity(int aqi)
{
    if (aqi >= 0 && aqi <= 50) {
        return 1;
    }else if(aqi > 50 && aqi <= 100){
        return 2;
    }else if(aqi > 100 && aqi <= 150){
        return 3;
    }else if(aqi > 150 && aqi <= 200){
        return 4;
    }else if(aqi > 200 && aqi <= 250){
        return 5;
    }else{
        return 6;
    }
    return -1;
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
        parseWeatherData(requestData);//解析json数据,创建2个类,ToDay和Day(数组)
        //解析完数据后，更新ui
        updateUi();
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

    //简化版
    mDay[0].minimumTemperature = parseString(yesterdayObj.value("low").toString());//最低温度
    mDay[0].maximumTemperature = parseString(yesterdayObj.value("high").toString());//最高温度
    mDay[0].fx = yesterdayObj.value("fx").toString();//风向
    mDay[0].fl = yesterdayObj.value("fl").toString();//风力
    qDebug() << mDay[0].date << mDay[0].aqi << mDay[0].type << mDay[0].week << mDay[0].minimumTemperature << mDay[0].maximumTemperature << mDay[0].fx << mDay[0].fl;
    //3.解析forecast中的6天数据
    //qDebug() << forecastArray.size();
    for (int i = 0; i < 5; ++i) {
        QJsonObject forecastData = forecastArray[i].toObject();
        //跳过0,因为0已经赋值给昨天的数据了
        mDay[i + 1].date = forecastData.value("date").toString();//日期
        mDay[i + 1].aqi = forecastData.value("aqi").toDouble();//空气质量,toDouble包括整数型
        mDay[i + 1].type = forecastData.value("type").toString();//天气类型
        mDay[i + 1].week = forecastData.value("week").toString();//星期
        mDay[i + 1].minimumTemperature = parseString(forecastData.value("low").toString());//最低温度
        mDay[i + 1].maximumTemperature = parseString(forecastData.value("high").toString());//最高温度
        mDay[i + 1].fx = forecastData.value("fx").toString();//风向
        mDay[i + 1].fl = forecastData.value("fl").toString();//风力
        mDay[i + 1].ymd = forecastData.value("ymd").toString();//完整年月日
        //ymd分割成MM/dd
        //QDateTime::fromString(forecastData.value("ymd").toString(), "yyyy-MM-dd").toString("MM/dd");//日期
        qDebug() << i << mDay[i + 1].date << mDay[i + 1].aqi << mDay[i + 1].type << mDay[i + 1].week << mDay[i + 1].minimumTemperature << mDay[i + 1].maximumTemperature << mDay[i + 1].fx << mDay[i + 1].fl;
        qDebug() << i << mDay[i].date;
        qDebug() << mDay[i + 1].ymd;
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
    mToday.minimumTemperature = mDay[1].minimumTemperature;
    mToday.maximumTemperature = mDay[1].maximumTemperature;
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

int Widget::parseString(QString str)
{
    str = str.split(" ").at(1);//以空格为分割符,去除前面的文字,取出第二个X℃,
    str = str.left(str.length() - 1);//去除后面的摄氏度符号
    return str.toInt();
}
