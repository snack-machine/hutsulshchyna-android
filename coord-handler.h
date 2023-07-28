#ifndef COORDHANDLER_H
#define COORDHANDLER_H

#include <QObject>

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

public:
    explicit CoordHandler(QObject* parent = nullptr);

    void Parse_GPGGA_Slot(const struct GPGGA& GPGGA);

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

public slots:
    void slotOpenBenchmarkFile(const QByteArray& data);

signals:
    void coordChanged();
};

#endif // COORDHANDLER_H
