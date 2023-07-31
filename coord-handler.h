#ifndef COORDHANDLER_H
#define COORDHANDLER_H

#include <QObject>
#include <QRegularExpression>
//#include "nmea_parser/NMEA_Struct.h"

class CoordHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString time READ getTime NOTIFY coordChanged)
    Q_PROPERTY(QString longitude READ getLongitude NOTIFY coordChanged)
    Q_PROPERTY(QString latitude READ getLatitude NOTIFY coordChanged)
    Q_PROPERTY(QString altitude READ getAltitude NOTIFY coordChanged)
    Q_PROPERTY(QString satelits READ getSatelits NOTIFY coordChanged)
    Q_PROPERTY(QString averageLong READ getAverageLong NOTIFY coordChanged)
    Q_PROPERTY(QString averageLat READ getAverageLat NOTIFY coordChanged)
    Q_PROPERTY(QString averageAlt READ getAverageAlt NOTIFY coordChanged)
//    Q_PROPERTY(QString dost READ getDost NOTIFY coordChanged)
//    QML_ELEMENT
//    struct GPGGA{
//        double   time;
//        double   latitude;
//        double   longitude;
//        double   altitude;
//        double   geo_factor;
//        double   H_geoid;
//        uint16_t diff;
//        char     lat;
//        char     lon;
//        char     alt;
//        char     h_geoid;
//        uint8_t  observation;
//        uint8_t  nka;
//    };
    struct GPGGA{
        double  time;
        double  latitude;
        double  longitude;
        double  altitude;
        QString lat;
        QString lon;
        QString alt;
    };

public:
    explicit CoordHandler(QObject* parent = nullptr);

//    void parseGPGAA(const CoordHandler::GPGGA& GPGGA);
    void parseGPGAA();
    Q_INVOKABLE void setCenter();

private:
//    static const QRegularExpression regex;

    const QString& getTime() const { return time; };
    const QString& getLongitude() const { return longitude; };
    const QString& getLatitude() const { return latitude; };
    const QString& getAltitude() const { return altitude; };
    const QString& getSatelits() const { return satelits; };
    const QString& getAverageLong() const { return averageLong; };
    const QString& getAverageLat() const { return averageLat; };
    const QString& getAverageAlt() const { return averageAlt; };
//    const QString& getDost() const { return dost; };

    double ddmmToDegrees(double value);

    struct GPGGA gpgga;

    QString time;
    QString latitude;
    QString longitude;
    QString altitude;
    QString satelits;
    QString averageLong;
    QString averageLat;
    QString averageAlt;
//    QString dost;

    double d_averageLat;
    double d_averageLong;
    double d_averageAlt;
    unsigned int size;
    //    double rmsd;

public Q_SLOTS:
    void getPortMessageSlot(const QByteArray& data);

Q_SIGNALS:
    void coordChanged();
};

#endif // COORDHANDLER_H
