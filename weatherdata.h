#ifndef WEATHERDATA_H
#define WEATHERDATA_H


#include <QString>

class TodayWeatherData
{
public:
    TodayWeatherData()
    {
        date = "2022-10-20";
        city = "广州";

        ganmao = "感冒指数";

        wendu = 0;
        shidu = "0%";
        pm25 = 0;
        quality = "无数据";

        type = "多云";

        fl = "2级";
        fx = "南风";

        maximumTemperature = 30;
        minimumTemperature = 18;
    }

    QString date;
    QString city;

    QString ganmao;

    int wendu;
    QString shidu;
    int pm25;
    QString quality;

    QString type;

    QString fx;
    QString fl;


    int maximumTemperature;
    int minimumTemperature;
};

class DayWeatherData
{
public:
    DayWeatherData()
    {
        date = "2024-1-10";
        week = "周五";

        type = "多云";

        maximumTemperature = 0;
        minimumTemperature = 0;

        fx = "南风";
        fl = "2级";

        aqi = 0;
    }

    QString date;
    QString week;

    QString type;
    QString ymd;

    int maximumTemperature;
    int minimumTemperature;

    QString fx;
    QString fl;

    int aqi;
};




#endif // WEATHERDATA_H
