#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H
#include<QJsonArray>
#include<QMap>
#include<QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

//天气工具类
class weatherTool{
private:
    inline static QMap<QString, QString> mCityMap = {};
    //将json文件加载进来，并将城市编码与城市名映射到map里
    static void initCityMap(){
        QFile fd(":/res/citycode.json");
        fd.open(QIODevice::ReadOnly | QIODevice::Text);
        //qDebug() << fd.isOpen();
        QByteArray json = fd.readAll();
        fd.close();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(json, &err);
        if (err.error != QJsonParseError::NoError || !doc.isArray()) {
            return;
        }
        QJsonArray rootArray = doc.array();
        //qDebug() << rootArray;
        for (int i = 0; i < rootArray.size(); ++i) {
            QString cityName = rootArray[i].toObject().value("city_name").toString();
            QString cityCode = rootArray[i].toObject().value("city_code").toString();
            //qDebug() << cityName << cityCode;
            //判断编号是否存在
            if (cityCode.size() > 0) {
                mCityMap.insert(cityName, cityCode);
            }
        }
    }

public:
    //根据城市名获取城市编号
    static QString getCityCode(QString cityName){
        //检查是否为空,为空先初始化
        if (mCityMap.isEmpty()) {
            initCityMap();
        }
        //在map寻找传入的城市名称,返回值可以遍历
        QMap<QString, QString>::iterator it = mCityMap.find(cityName);
        //到结尾了,未找到,加上市,再搜一遍
        if (it == mCityMap.end()) {
            it = mCityMap.find(cityName + "市");
        }
        //找到了
        if (it != mCityMap.end()) {
            return it.value();
        }
        //还是未找到,返回空值
        return "";
    }
};
#endif // WEATHERTOOL_H
