
#include "coord-handler.h"

#include <QDebug>
#include <QTime>

//const QRegularExpression CoordHandler::regex("^\\$.{2}GGA");

CoordHandler::CoordHandler(QObject* parent)
    : QObject{parent}, d_averageLat{0}, d_averageLong{0}, d_averageAlt{0}, size{0}
{
}

void CoordHandler::parseGPGAA()
{
//    time = QString::number(GPGGA.time);

//    if(GPGGA.time < 100000)
//        time.push_front('0');
//    time.push_back("00");

//    time[7] = time.at(5);
//    time[6] = time.at(4);
//    time[5] = ':';
//    time[4] = time.at(3);
//    time[3] = time.at(2);
//    time[2] = ':';
//    time.push_back("  ");

//    latitude = QString::number(GPGGA.latitude, 'g', 9) + GPGGA.lat;
//    longitude = QString::number(GPGGA.longitude, 'g', 9) + GPGGA.lon;
//    altitude = QString::number(GPGGA.altitude, 'f', 2) + GPGGA.alt;
////    satelits = QString::number(GPGGA.nka);

//    d_averageLat  = (GPGGA.latitude  + d_averageLat * size) / (size + 1);
//    d_averageLong = (GPGGA.longitude + d_averageLat * size) / (size + 1);
//    d_averageAlt = (GPGGA.altitude + d_averageAlt * size) / (size + 1);
//    size++;

////    visual_coord->Set_Average_Slot(A_Lat, A_Long);
//    averageLat = QString::number(d_averageLat, 'f', 2);
//    averageLong = QString::number(d_averageLong, 'g', 9);
//    averageAlt  = QString::number(d_averageAlt, 'g', 9);
//    emit coordChanged();

    latitude = QString::number(gpgga.latitude, 'g', 9) + gpgga.lat;
    longitude = QString::number(gpgga.longitude, 'g', 9) + gpgga.lon;
    altitude = QString::number(gpgga.altitude, 'f', 2) + gpgga.alt;
    //    satelits = QString::number(gpgga.nka);

    d_averageLat  = (gpgga.latitude  + d_averageLat * size) / (size + 1);
    d_averageLong = (gpgga.longitude + d_averageLat * size) / (size + 1);
    d_averageAlt = (gpgga.altitude + d_averageAlt * size) / (size + 1);
    size++;

    //    visual_coord->Set_Average_Slot(A_Lat, A_Long);
    averageLat = QString::number(d_averageLat, 'f', 2);
    averageLong = QString::number(d_averageLong, 'g', 9);
    averageAlt  = QString::number(d_averageAlt, 'g', 9);
    emit coordChanged();
//    Average_Alt_VL->setText(Average_Alt + GPGGA.h_antena);

//    double RMS_Lat =  (this->A_Lat  - GPGGA.latitude)*Grad_to_M;
//    double RMS_Long = (this->A_Long - GPGGA.longitude) *Grad_to_M*cos(this->A_Lat);

//    double Distance = RMS_Lat*RMS_Lat + RMS_Long*RMS_Long;

//    RMS = (Distance + RMS*(size-1))/size;

//    RMS_VL->setText(QString::number(sqrt(RMS), 'g', 2));

//    switch(GPGGA.observation)
//    {
//    case 0:
//        this->Dost = tr("Fix not available\n or invalid.");
//        break;
//    case 1:
//        this->Dost = tr("Position fix valid.\nAutonomous mode.");
//        break;
//    case 2:
//        this->Dost = tr("Position fix valid.\nDifferential mode.");
//        break;
//    case 3:
//        this->Dost = tr("Estimated data\n(extrapolation, dead reckoning)");
//        break;
//    }
//    Dost_VL->setText(this->Dost);
//    Dost_VL->setFont(FON);

//    visual_coord->animate(GPGGA);
}

//void Coord_QW::Clear_Slot()
//{
//    this->A_Lat = 0;
//    this->A_Long = 0;
//    this->size = 0;

//    Time_VL         ->setText("0");
//    Long_VL         ->setText("0");
//    Lat_VL          ->setText("0");
//    Satelits_VL     ->setText("0");
//    Dost_VL         ->setText(tr("___________________"));
//    Average_Long_VL ->setText("0");
//    Average_Lat_VL  ->setText("0");
//    RMS_VL          ->setText("0");

//    visual_coord->Set_Average_Slot(0, 0);
//    visual_coord->repaint();
//}

void CoordHandler::setCenter()
{
//    visual_coord->Set_Center_Lat_Slot(A_Lat);
//    visual_coord->Set_Center_Long_Slot(A_Long);

//    Lat_Spinbox ->setValue(A_Lat);
//    Long_Spinbox->setValue(A_Long);
}

//void CoordHandler::getPortMessageSlot(const QByteArray& data)
//{
//    QString Text_NMEA = QString::fromLocal8Bit(data);
//    QStringList Text_NMEA_List = Text_NMEA.split("\n");

//    for(int i = 0; i < Text_NMEA_List.size(); i++) {
//        if(Text_NMEA_List.at(i).size() > 0) {
//            QString Z = *(Text_NMEA_List.at(i).cend() - 1);

//            if(Z != "\r") {
//                Text_NMEA_List[i] += "\r";
//            }

//            Text_NMEA = Text_NMEA_List.at(i) + "\n";

//            NMEA_D = NMEA_Data_Settings;
//            char data2[4096];
////            string data2{Text_NMEA.toLocal8Bit().data()};
////            nmea_recv(&ctx, data2.c_str(), data2.size(), &nmead);
//            memset(data2, 0, std::size(data2));
//            strncpy(data2, Text_NMEA.toLocal8Bit().data(), std::size(data2));

//            qDebug() << data2;
//            nmea_recv(&ctx, data2, strlen(data2), &NMEA_D);
//            qDebug() << NMEA_D.GPGGA.latitude <<  NMEA_D.GPGGA.longitude;
//            if(NMEA_D.GPGGA.latitude && NMEA_D.GPGGA.longitude) {
//                qDebug() << "yes";
//                parseGPGAA(NMEA_D.GPGGA);
//            }
//        }
//    }
//}

void CoordHandler::getPortMessageSlot(const QByteArray& data)
{
    const QString stringData = QString::fromLocal8Bit(data);
    const QStringView dataView{stringData.trimmed()};

    for (QStringView line : dataView.split('\n')) {
        qDebug() << "STTTTAAAAAARRRRTTT";
        qDebug() << line.length();
//        qDebug() << line.length();
        if (line.length() < 10) {
            continue;
        }
//        bool regMatch = regex.match(line).hasMatch();
//        if (!regMatch) {
//            continue;
//        }
        if (line.at(0) != '$' || line.at(3) != 'G' || line.at(4) != 'G' || line.at(5) != 'A') {
            continue;
        }

        const QList<QStringView> tokens = line.split(',');
        if (tokens.size() < 13) {
            continue;
        }
        /* check checksum */

        QTime parsedTime = QTime::fromString(tokens.at(1).toString(), "HHmmss.z");
        time = parsedTime.toString();
        bool ok = false;
        double lat  = tokens.at(2).toDouble(&ok);
        double lng = tokens.at(4).toDouble(&ok);
        if (!ok) {
            continue;
        }
        gpgga.latitude = ddmmToDegrees(lat);
        gpgga.longitude = ddmmToDegrees(lng);
        gpgga.altitude = tokens.at(9).toDouble();

        gpgga.lat = tokens.at(3).toString();
        gpgga.lat = tokens.at(5).toString();
        gpgga.lat = tokens.at(10).toString();

        parseGPGAA();
    }    
}

double CoordHandler::ddmmToDegrees(double value) {
    int degrees = static_cast<int>(value / 100);
    double minutes = value - degrees * 100;
    return degrees + (minutes / 60.0);
}
