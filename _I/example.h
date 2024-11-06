#pragma once
#include "packer.h"
 
namespace example
{


typedef struct __attribute__((packed))
{
    //% bits=9
    int16_t         NAGR;       //�������� ��������, ��������
    //% bits=12
    uint16_t        GKup;       //�������� ������
    //% bits=12
    uint16_t        GKdn;       //�������� �����
} skv_t;

typedef struct __attribute__((packed))
{
    //% to=2000
    //% bits=10
    int16_t        X; //��� X �������������
    //% to=2000
    //% bits=10
    int16_t        Y; //��� Y �������������
    //% to=2000
    //% bits=10
    int16_t        Z; //��� Z �������������
} accel_t;

//% send
typedef struct  __attribute__((packed))
{
    //% from=0
    //% to=95
    //% bits=10
    ufloat          ZU;             //� ��������
    //% kmul=10
    //% bits=9
    ufloat          GKup;           //� ��� � ��������
    //% from=0
    //% to=3000
    //% bits=7 
    uint16_t        OBOR;           //��/��� ��������
    float           NAGR;           //� ��, ��������, +-32����� 
    //% from=-20
    //% to=150
    //% bits=7
    float           TEMP;           //� ��������
    skv_t           skv;            //��������
    accel_t         accel;          //���i �������������
    int16_t         MOMENT;         //�*� +-32768
    float           MOMENT_skv;
    //% kmul=10
    ufloat          DAVL;            // ��� � ��������
    //% from=-20
    //% to=150
    //% bits=8
    float           TEMPd;           //� ��������
    //% to=300
    //% bits=9
    ufloat          GKdn;            //� ��� � ��������
} ExampleToSend_t;

}
