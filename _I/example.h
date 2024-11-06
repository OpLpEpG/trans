#pragma once
#include "packer.h"
 
namespace example
{


typedef struct __attribute__((packed))
{
    //% bits=9
    int16_t         NAGR;       //Сквозняк Нагрузки, знаковое
    //% bits=12
    uint16_t        GKup;       //Сквозняк ГКверх
    //% bits=12
    uint16_t        GKdn;       //Сквозняк ГКниз
} skv_t;

typedef struct __attribute__((packed))
{
    //% to=2000
    //% bits=10
    int16_t        X; //Ось X акселерометра
    //% to=2000
    //% bits=10
    int16_t        Y; //Ось Y акселерометра
    //% to=2000
    //% bits=10
    int16_t        Z; //Ось Z акселерометра
} accel_t;

//% send
typedef struct  __attribute__((packed))
{
    //% from=0
    //% to=95
    //% bits=10
    ufloat          ZU;             //с десятыми
    //% kmul=10
    //% bits=9
    ufloat          GKup;           //в мкР с десятыми
    //% from=0
    //% to=3000
    //% bits=7 
    uint16_t        OBOR;           //об/мин напрямую
    float           NAGR;           //в кг, знаковое, +-32тонны 
    //% from=-20
    //% to=150
    //% bits=7
    float           TEMP;           //с десятыми
    skv_t           skv;            //Сквозняк
    accel_t         accel;          //Осьi акселерометра
    int16_t         MOMENT;         //Н*м +-32768
    float           MOMENT_skv;
    //% kmul=10
    ufloat          DAVL;            // атм с десятыми
    //% from=-20
    //% to=150
    //% bits=8
    float           TEMPd;           //с десятыми
    //% to=300
    //% bits=9
    ufloat          GKdn;            //в мкР с десятыми
} ExampleToSend_t;

}
