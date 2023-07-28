
#include "coord-handler.h"
#include "nmea_parser/NMEA_Struct.h"

CoordHandler::CoordHandler(QObject* parent)
    : QObject{parent}, d_averageLat{0}, d_averageLong{0}, d_averageAlt{0}, size{0}
{

}

void CoordHandler::Parse_GPGGA_Slot(const struct GPGGA& GPGGA)
{
    time = QString::number(GPGGA.time);

    if(GPGGA.time < 100000)
        time.push_front('0');
    time.push_back("00");

    time[7] = time.at(5);
    time[6] = time.at(4);
    time[5] = ':';
    time[4] = time.at(3);
    time[3] = time.at(2);
    time[2] = ':';
    time.push_back("  ");

//    Time_VL->setText(this->Time);

    latitude   = QString::number(GPGGA.latitude, 'g', 9) + GPGGA.lat;
//    Lat_VL->setText(this->Lat);

    longitude  = QString::number(GPGGA.longitude, 'g', 9) + GPGGA.lon;
//    Long_VL->setText(this->Long);

    altitude  = QString::number(GPGGA.H_antena, 'f', 2) + GPGGA.h_antena;
//    Alt_VL->setText(this->Alt);

    satelits = QString::number(GPGGA.nka);
//    Satelits_VL->setText(this->Satelits);

    d_averageLat  = (GPGGA.latitude  + d_averageLat * size) / (size + 1);
    d_averageLong = (GPGGA.longitude + d_averageLat * size)/ (size + 1);
    d_averageAlt = (GPGGA.H_antena + d_averageAlt * size) / (size + 1);
    size++;

//    visual_coord->Set_Average_Slot(A_Lat, A_Long);

    averageLat = QString::number(d_averageLat, 'f', 2);
//    Average_Lat_VL->setText(Average_Lat + GPGGA.lat);
    averageLong = QString::number(d_averageLong, 'g', 9);
//    Average_Long_VL->setText(Average_Long + GPGGA.lon);
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

//void Coord_QW::Set_Center()
//{
//    visual_coord->Set_Center_Lat_Slot(A_Lat);
//    visual_coord->Set_Center_Long_Slot(A_Long);

//    Lat_Spinbox ->setValue(A_Lat);
//    Long_Spinbox->setValue(A_Long);
//}

//connect(parse_nmea,         SIGNAL(Parse_GPGGA_Signal(const GPGGA&)),
//        coord,              SLOT(Parse_GPGGA_Slot(const GPGGA&)));

void CoordHandler::slotOpenBenchmarkFile(const QByteArray& data)
{

    QString Text_NMEA = QString::fromLocal8Bit(data);
    QStringList Text_NMEA_List = Text_NMEA.split("\n");

    struct NMEA_Data NMEA_D, NMEA_Data_Settings;
    memset(&NMEA_Data_Settings, 0, sizeof(struct NMEA_Data));
    NMEA_Data_Settings.GPGGA.time = 1;
    BmUartProtoNmea ctx;
    memset(&ctx, 0, sizeof(BmUartProtoNmea));

    for(int i = 0; i < Text_NMEA_List.size(); i++) {
        if(Text_NMEA_List.at(i).size() > 0) {
            QString Z = *(Text_NMEA_List.at(i).cend() - 1);

            if(Z != "\r") {
                Text_NMEA_List[i] += "\r";
            }

            Text_NMEA = Text_NMEA_List.at(i) + "\n";

            NMEA_D = NMEA_Data_Settings;
            char data2[4096];
            memset(data2, 0, 4096);
            strncpy(data2, Text_NMEA.toLocal8Bit().data(), sizeof(data2));
            nmea_recv(&ctx, data2, strlen(data2), &NMEA_D);
            if(NMEA_D.GPGGA.latitude && NMEA_D.GPGGA.longitude) {
                Parse_GPGGA_Slot(NMEA_D.GPGGA);
            }
        }
    }
}
