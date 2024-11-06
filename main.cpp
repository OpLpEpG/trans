/*
 * trans.cpp
 *
 * Created: 12.09.2024 8:14:58
 * Author : User
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <ccp.h>

#include <Indicator.hpp>

#include "ndm.h"
#include "GenExample.h"
#include "transmit.h"


static int8_t CLKCTRL_init();

ndm_t sendData;

CREATE_TRANSMITTER(LEN_ARRAY5BIT, tr)

uint16_t takt;
volatile bool flagSend;
ISR(TCB3_INT_vect)
{
	TCB3.INTFLAGS = TCB_CAPT_bm | TCB_OVF_bm;
	if (takt++ == 999)
	{
		takt = 0;
		flagSend = true;
		// можно начать передавать сразу (пока передаются: преамбула, СП) начинается упаковка данных (Encode)
		// т.к. Encode + прерывания на передачу данных, выполняются быстрее, чем передаются данные
		tr.Send();
	}	
}
	

int main(void)
{
	PORTC.DIRSET = 0b00110000;
	CLKCTRL_init();
	
	TCB3.INTFLAGS = TCB_CAPT_bm | TCB_OVF_bm;
	TCB3.CTRLA = TCB_CLKSEL_DIV2_gc     /* CLK_PER/2 */
	| 1 << TCB_ENABLE_bp   /* Enable: enabled */
	| 1 << TCB_RUNSTDBY_bp /* Run Standby: enabled */
	| 0 << TCB_SYNCUPD_bp  /* Synchronize Update: disabled */
	| 0 << TCB_CASCADE_bp; /* Cascade Two Timer/Counters: disabled */
	//1Khz 
	TCB3.CCMP =  8000-1; 
	TCB3.INTCTRL = 1 << TCB_CAPT_bp   /* Capture or Timeout: disabled */
	| 0 << TCB_OVF_bp; /* OverFlow Interrupt: enabled */
	
   	sei();
    
    sendData.ZU = 12.45;
	sendData.v_bat = 5.1;
    sendData.GKdn = 100.9;
	sendData.GKup = 93.4;
	sendData.OBOR = 100;
	sendData.tok_trans = 715;
	sendData.TEMP = 97.3;
	sendData.v_trans = 16.678;
	sendData.tok_trans = 13.1;
	sendData.MOMENTtr = 20.1;
	sendData.DAVLtr = 200;
	sendData.NAGR =-1.3;
	sendData.mah = 1.7;
	sendData.flag = 1;
    
    while (1) {
		
		if (flagSend) 
		{					
			flagSend = false;
		
			//if(tr.flagBusy == 0)
			{			
				Indicator.On();
			
				sendData.count_kadr++;			

				Encode(&sendData,tr.SendDataPtr());
						
				//sendData.v_bat -= 0.1;			
				sendData.flag = ~sendData.flag & 1;			
			
				//while (tr.flagBusy); для индикации всего времени передачи
			
				Indicator.Off();		
				//_delay_ms(1000);	
			}
			//else
			//{
				//Indicator.On();		
				//while(1);		
			//}
		}
    }	
}
#define CLKCTRL_FREQSEL_16M_gc (0x07<<2) /* 16 MHz system clock */
static int8_t CLKCTRL_init()
{

	// ccp_write_io((void*)&(CLKCTRL.XOSC32KCTRLA),CLKCTRL_CSUT_1K_gc /* 1k cycles */
	//		 | 0 << CLKCTRL_ENABLE_bp /* Enable: disabled */
	//		 | 0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */
	//		 | 0 << CLKCTRL_SEL_bp /* Select: disabled */
	//		 | 0 << CLKCTRL_LPMODE_bp /* Low-Power Mode: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKCTRLB),CLKCTRL_PDIV_2X_gc /* 2 */
	//		 | 0 << CLKCTRL_PEN_bp /* Prescaler enable: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.PLLCTRLA),0 << CLKCTRL_RUNSTDBY_bp /* Run Standby: disabled */
	//		 | CLKCTRL_MULFAC_DISABLE_gc /* 1 */
	//		 | 0 << CLKCTRL_SOURCE_bp /* Select Source for PLL: disabled */);

	ccp_write_io((void *)&(CLKCTRL.MCLKCTRLC),
	CLKCTRL_CFDSRC_CLKMAIN_gc /* Main Clock */
	| 1 << CLKCTRL_CFDEN_bp /* Setting: enabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKINTCTRL),CLKCTRL_INTTYPE_INT_gc /* Regular Interrupt */
	//		 | 0 << CLKCTRL_CFD_bp /* Clock Failure Detection Interrupt Enable: disabled */);

	ccp_write_io((void *)&(CLKCTRL.XOSCHFCTRLA),
	1 << CLKCTRL_RUNSTDBY_bp      /* Run standby: enabled */
	| CLKCTRL_CSUTHF_256_gc   /* 256 XOSCHF cycles */
	| CLKCTRL_FRQRANGE_16M_gc /* Max 16 MHz XTAL Frequency */
	| 0 << CLKCTRL_SEL_bp     /* Source Select: disabled */
	| 1 << CLKCTRL_ENABLE_bp /* External high-frequency Oscillator: enabled */);

	ccp_write_io((void *)&(CLKCTRL.OSCHFCTRLA),
	CLKCTRL_FREQSEL_16M_gc         /* 16 */
	| 0 << CLKCTRL_AUTOTUNE_bp /* Auto-Tune enable: disabled */
	| 0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */);

	ccp_write_io((void *)&(CLKCTRL.MCLKCTRLA),
	CLKCTRL_CLKSEL_EXTCLK_gc /* External clock */
	| 0 << CLKCTRL_CLKOUT_bp /* System clock out: disabled */);

	/* wait for system oscillator changing to finish */
	while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
	;

	return 0;
}
