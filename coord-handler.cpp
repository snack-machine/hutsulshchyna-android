
#include "coord-handler.h"

#include <QTime>
//const QRegularExpression CoordHandler::regex("^\\$.{2}GGA");
//#include <QDebug>

CoordHandler::CoordHandler(QObject* parent)
    : QObject{parent}, d_averageLat{0}, d_averageLong{0}, d_averageAlt{0}, size{0}
{
}

void CoordHandler::parseGPGAA()
{
    latitude = QString::number(gpgga.latitude, 'g', 8) + gpgga.lat;
    longitude = QString::number(gpgga.longitude, 'g', 8) + gpgga.lon;
    altitude = QString::number(gpgga.altitude, 'f', 2) + gpgga.alt;

    d_averageLat  = (gpgga.latitude  + d_averageLat * size) / (size + 1);
    d_averageLong = (gpgga.longitude + d_averageLong * size) / (size + 1);
    d_averageAlt = (gpgga.altitude + d_averageAlt * size) / (size + 1);
    size++;

    averageLat = QString::number(d_averageLat, 'g', 8);
    averageLong = QString::number(d_averageLong, 'g', 8);
    averageAlt  = QString::number(d_averageAlt, 'f', 2);
    emit coordChanged();
}

void CoordHandler::setCenter()
{
//    visual_coord->Set_Center_Lat_Slot(A_Lat);
//    visual_coord->Set_Center_Long_Slot(A_Long);

//    Lat_Spinbox ->setValue(A_Lat);
//    Long_Spinbox->setValue(A_Long);
}

void CoordHandler::clearData()
{
    latitude.clear();
    longitude.clear();
    altitude.clear();

    d_averageLat = 0;
    d_averageLong = 0;
    d_averageAlt = 0;
    size = 0;

    averageLat.clear();
    averageLong.clear();
    averageAlt.clear();

    time.clear();
    emit coordChanged();
}

void CoordHandler::getPortMessageSlot(const QByteArray& data)
{
    if (data.size() < 10) {
        return;
    }

    if (data.at(0) != '$' || data.at(3) != 'G' || data.at(4) != 'G' || data.at(5) != 'A') {
        return;
    }

    /* check checksum */
    int crc = 0;
    qsizetype end = data.lastIndexOf('*');
    for (qsizetype i = 1; i < end; ++i) {
        crc ^= data.at(i);
    }
    bool ok = false;
    int crcPart = data.mid(end+1, 2).toInt(&ok, 16);
    if (crc != crcPart) {
        return;
    }

    const QString stringData = QString::fromLocal8Bit(data);
    const QStringView line{stringData};

    const QList<QStringView> tokens = line.split(',');
    if (tokens.size() < 11) {
        return;
    }
    QTime parsedTime = QTime::fromString(tokens.at(1).toString(), "HHmmss.z");
    time = parsedTime.toString();

    double lat  = tokens.at(2).toDouble(&ok);
    double lng = tokens.at(4).toDouble(&ok);
    if (!ok) {
        return;
    }
    gpgga.latitude = ddmmToDegrees(lat);
    gpgga.longitude = ddmmToDegrees(lng);
    gpgga.altitude = tokens.at(9).toDouble();

    gpgga.lat = tokens.at(3).toString();
    gpgga.lon = tokens.at(5).toString();
    gpgga.alt = tokens.at(10).toString();

    parseGPGAA();
}

double CoordHandler::ddmmToDegrees(double value) {
    int degrees = static_cast<int>(value / 100);
    double minutes = value - degrees * 100;
    return degrees + (minutes / 60.0);
}
