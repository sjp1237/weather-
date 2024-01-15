#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMenu>
#include<QAction>

//菜单
#include <QContextMenuEvent>
//鼠标事件
#include <QMouseEvent>
//网络请求
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include<QLabel>
#include"weatherdata.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    //退出的菜单
    QMenu* mExitMenu;
    //退出的行为
    QAction* mExitAction;
    //鼠标点击的pos距离左上角的pos的偏移

    //鼠标点击的pos距离左上角的pos的偏移
    QPoint mOffset;

    QNetworkAccessManager* mNetworkAccessManager;

    //今天的数据
    TodayWeatherData mToday;
    //未来几天的数据
    DayWeatherData mDay[6];

    QList<QLabel*> mDateList;
    QList<QLabel*> mWeekList;
    QList<QLabel*> mQulityList;
    QList<QLabel*> mTypeList;
    QList<QLabel*> mTypeIconList;
    QList<QLabel*> mFxList;
    QList<QLabel*> mFlList;
    QMap<QString, QString> mTypeMap;

    void ComponentInit();

    //获取天气数据
    void getWeatherInfo(QString cityName);

    //解析天气数据
    void parseWeatherData(QByteArray data);

    int parseString(QString str);

    void updateUi();

    int checkedQulity(int aqi);
    //鼠标右键事件
    void contextMenuEvent(QContextMenuEvent *event);
     //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    // //鼠标移动事件;
    void mouseMoveEvent(QMouseEvent *event);




  private slots:
    void onReplied(QNetworkReply *reply);
    // //事件过滤器,默认无,需要重写
    // bool eventFilter(QObject *watched, QEvent *event);


};
#endif // WIDGET_H
