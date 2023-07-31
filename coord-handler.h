#ifndef COORDHANDLER_H
#define COORDHANDLER_H

#include <QObject>
#include "nmea_parser/NMEA_Struct.h"

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
    Q_PROPERTY(QString dost READ getDost NOTIFY coordChanged)
//    QML_ELEMENT

public:
    explicit CoordHandler(QObject* parent = nullptr);

    void parseGPGAA(const struct GPGGA& GPGGA);
    Q_INVOKABLE void setCenter();

private:
    const QString& getTime() const { return time; };
    const QString& getLongitude() const { return longitude; };
    const QString& getLatitude() const { return latitude; };
    const QString& getAltitude() const { return altitude; };
    const QString& getSatelits() const { return satelits; };
    const QString& getAverageLong() const { return averageLong; };
    const QString& getAverageLat() const { return averageLat; };
    const QString& getAverageAlt() const { return averageAlt; };
    const QString& getDost() const { return dost; };

    QString time;
    QString longitude;
    QString latitude;
    QString altitude;
    QString satelits;
    QString dost;
    QString averageLong;
    QString averageLat;
    QString averageAlt;

    double d_averageLat;
    double d_averageLong;
    double d_averageAlt;
    unsigned int size;
    //    double rmsd;

    struct NMEA_Data NMEA_D, NMEA_Data_Settings;
    BmUartProtoNmea ctx;

public Q_SLOTS:
    void getPortMessageSlot(const QByteArray& data);

Q_SIGNALS:
    void coordChanged();
};

#endif // COORDHANDLER_H
