/*
 * ndm.h
 *
 * Created: 26.09.2024 10:26:27
 *  Author: User
 */ 


#ifndef NDM_H_
#define NDM_H_

#include "packer.h"

//% send
typedef struct  __attribute__((packed))
{
	 //% bits =1;
	 uint8_t flag;
	 //% bits =6;
	 //% kmul = 10
	 //% delta = -1.1
	 ufloat v_bat;
	 //% bits =8;
	 //% kmul = 10
	 ufloat mah;
	 //% bits =10;
	 //% kmul = 10
	 ufloat        GKup;	//� ��� � ��������
	 //% bits =20;
	 uint32_t		count_kadr;
	 //% bits = 11;
	 //% kmul = 10
	 ufloat        ZU;	//� ��������
	 //% bits =9;
	 ufloat        OBOR;	//��/��� ��������
	 //% bits =9;
	 //% kmul = 10
	 float         NAGR;	//� ��, ��������, +-32�����
	 //% bits =11;
	 //% kmul = 10
	 ufloat         TEMP;	//� ��������
	 //% bits =15;
	 //% kmul = 1000
	 ufloat v_trans;
	 //% bits =12;
	 ufloat tok_trans;
	 //% bits =9;
	 //% kmul = 10
	 ufloat MOMENTtr;
	 //% bits =9;
	 int16_t DAVLtr;
	 //% bits =10;
	 //% kmul = 10
	 ufloat        GKdn;	//� ��� � ��������
} ndm_t;



#endif /* NDM_H_ */