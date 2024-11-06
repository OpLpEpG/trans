/*
 * transmit.h
 *
 * Created: 23.08.2024 9:18:54
 *  Author: User
 */ 


#ifndef TRANSMIT_H_
#define TRANSMIT_H_
#include <avr/io.h>
#include "tools.h"

const uint32_t _digkod[38]=
{
	0xa95a9a69, 0x565a6569, 0x59aa6a99, 0xa6aa9599, 0x659656a5, 0x9a96a9a5, 0x9566a655, 0x6a665955,
	0x5556999a, 0x55a99965, 0x5a599695, 0x5aa6966a, 0x6665aaa9, 0x669aaa56, 0x696aa5a6, 0x6995a559,
	(uint32_t)~0xa95a9a69,(uint32_t)~0x565a6569,(uint32_t)~0x59aa6a99,(uint32_t)~0xa6aa9599,(uint32_t)~0x659656a5,(uint32_t)~0x9a96a9a5,(uint32_t)~0x9566a655,(uint32_t)~0x6a665955,
	(uint32_t)~0x5556999a,(uint32_t)~0x55a99965,(uint32_t)~0x5a599695,(uint32_t)~0x5aa6966a,(uint32_t)~0x6665aaa9,(uint32_t)~0x669aaa56,(uint32_t)~0x696aa5a6,(uint32_t)~0x6995a559,
	0x55555555,//razgon
	0x6999a656, 0x99559655, 0xa59556a9, 0x5a595995, //SP
	0xaaaaaaaa,//konec
};

#define CREATE_TRANSMITTER(len, name) transmit_t<len> name; ISR(TCB0_INT_vect) {name._ISRSend();}

template <uint8_t CNLS>
class transmit_t
{
private:
	static const uint8_t BufLen  =  (1+4+CNLS+1);
    uint8_t BitData[BufLen];
	unio32_t _CurrentCode;
    uint8_t _idxPacket;
    uint8_t _idxBit;	
	
	INLN void _TIMER_1_init()
	{
		TCB0.INTFLAGS = TCB_CAPT_bm | TCB_OVF_bm;
		TCB0.CTRLA = TCB_CLKSEL_DIV2_gc     /* CLK_PER/2 */
		| 1 << TCB_ENABLE_bp   /* Enable: enabled */
		| 1 << TCB_RUNSTDBY_bp /* Run Standby: enabled */
		| 0 << TCB_SYNCUPD_bp  /* Synchronize Update: disabled */
		| 0 << TCB_CASCADE_bp; /* Cascade Two Timer/Counters: disabled */
	}
		
public:
	
    volatile uint8_t flagBusy;
	
	INLN uint8_t* SendDataPtr(void)  { return &BitData[5]; } 

	INLN void _ISRSend(void)
	{
		PORTC.OUTCLR = 0b00110000;
	
		PORTC.OUTSET = (_CurrentCode.B[3] & 0x80) ? 0b00100000 : 0b00010000 ;
		
		TCB0.INTFLAGS = TCB_CAPT_bm | TCB_OVF_bm;

		if (++_idxBit == 32)
		{
			_idxBit = 0;
			if (++_idxPacket == BufLen)
			{
				TCB0.CTRLA = 0;
				PORTC.OUTCLR = 0b00110000; // последний бит (konec) теряем
				_idxPacket = 0;
				flagBusy = 0;
				return;
			}
			else _CurrentCode.u32 = _digkod[BitData[_idxPacket]];
		}
		else _CurrentCode.u32 <<= 1;
	}
	
	INLN transmit_t(void)
	{
		//31.25Kbit/s-512
		//15.625Kbit/s-1024
		//40Kbit.s = 400
		TCB0.CCMP = 400-1;// 800-1; /* Compare or Capture: 0x31f */
		//TCB0.CNT = 0; /* Count: 0x0 */
		TCB0.INTCTRL = 1 << TCB_CAPT_bp   /* Capture or Timeout: disabled */
		| 0 << TCB_OVF_bp; /* OverFlow Interrupt: enabled */		
		for(uint8_t i=0; i < 5;  i++) BitData[ i ] = i + 32;
		//for(uint8_t i=5; i < 28+5;  i++) BitData[ i ] = i-5;
		BitData[BufLen-1] = 37;
	}
	INLN void Send(void)
	{
		_CurrentCode.u32 = _digkod[32];
		flagBusy = 1;
		_TIMER_1_init();
	}
	
};

//extern unio32_t _CurrentCode;
////extern uint8_t _CurrentByte;
//extern uint8_t _idxPacket;
////extern uint8_t _idxByte;
//extern uint8_t _idxBit;
//
////extern uint16_t _TestdxxPack;
//
//
//extern volatile uint8_t TRANS_flagBusy;
//extern uint8_t TRANS_5BitData[TRANS_5B_LEN];
//
//extern void TRANS_Init(void);
//extern void TRANS_Send(void);
//
//INLN void TRANS_ISR_Send(void)
//{
////	_TestdxxPack++;
	//PORTC.OUTCLR = 0b00110000;
	//
	//PORTC.OUTSET = (_CurrentCode.B[3] & 0x80) ? 0b00100000 : 0b00010000 ;
	//if (++_idxBit == 32)
	//{
		//_idxBit = 0;
		//if (++_idxPacket == TRANS_5B_LEN)
		//{
			//_idxPacket = 0;
			//PORTC.OUTCLR = 0b00110000; // последний бит (konec) теряем
			//TCB0.CTRLA = 0;
			//TRANS_flagBusy = 0;
			//return;
		//}
		//else _CurrentCode.u32 = _digkod[TRANS_5BitData[_idxPacket]];
	//}
	//else _CurrentCode.u32 <<= 1;
	//
	////PORTC.OUTSET = (_CurrentByte & 0x80) ? 0b00010000 : 0b00100000;
	////if (++_idxBit == 8)
	////{
		////_idxBit = 0;
		////if (++_idxByte == 4)
		////{
			////_idxByte = 0;
			////if (++_idxPacket == TRANS_5B_LEN)
			////{
				////_idxPacket =0;
				////PORTC.OUTCLR = 0b00110000; // последний бит (konec) теряем
   				////TCB0.CTRLA = 0;
				////TRANS_flagBusy = 0;
				////return;
			////}
			////else _CurrentCode.u32 = _digkod[TRANS_5BitData[_idxPacket]];
		////}		
		////_CurrentByte = _CurrentCode.B[_idxByte];
	////}
	////else _CurrentByte <<= 1;
//}



#endif /* TRANSMIT_H_ */