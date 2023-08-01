#ifndef PARSENMEA_H
#define PARSENMEA_H

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

struct GPGGA{
    double   time;
    double   latitude;
    char     lat;
    double   longitude;
    char     lon;
    uint8_t  observation;
    uint8_t  nka;
    double   geo_factor;
    double   H_antena;
    char     h_antena;
    double   H_geoid;
    char     h_geoid;
    uint16_t diff;
};

struct POHPR{
    double  time;
    float   course;
    float   roll;
    float   pitch;
    char    status;
};

struct GPRMC{
    float       Time;
    char        Status;
    float       Lat;
    float       Long;
    float       Speed;
    float       Course;
    uint32_t    Date;
    float       Magnit;
    char        Magnit_Varitation;
    char        Mode;
    char        Navigation_Status;
};


struct NMEA_Data{               // GPGSA GPGSV
    struct GPGGA    GPGGA;      // confirmed
    struct POHPR    POHPR;      // ask
    struct GPRMC    RMC;        // confirmed
};

typedef struct {
    union {
        uint8_t nfields;
        char    data[82];
    };
    char       *cursor;
} BmNmeaMessage;

typedef struct {

    char manufacturer_name[16];
    char device_name[16];

    BmNmeaMessage rx;

} BmUartProtoNmea;

#ifdef __cplusplus

extern "C" {

void nmea_recv(void *, const char *, size_t , struct NMEA_Data *);

unsigned NmeaFormat(char *dst, unsigned dstlen, const char *fmt, ...);
}

#endif

#endif // PARSENMEA_H
