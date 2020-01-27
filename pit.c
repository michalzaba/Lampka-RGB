#include "pit.h"
#include "led.h"
volatile uint8_t toogle;
void PitInitialize(void)
{

	
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; //clock to PIT
  PIT->MCR = 0x00;   //enable PIT module
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR |= PIT_MCR_FRZ_MASK;	
	
	//load value
  //PIT->CHANNEL[0].LDVAL = 0x1234567;
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(0x2090000);

	
	
	//starting interrupt service
	PIT->CHANNEL[0].TCTRL |=PIT_TCTRL_TIE_MASK;	

	
		//interrupt priority
	NVIC_SetPriority(PIT_IRQn,2);
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);
	

	//timer start
		PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
		 toogle=0x00;
}



void PIT_IRQHandler()
{volatile int delay;

	NVIC_ClearPendingIRQ(PIT_IRQn);
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
	{
		 if(!buttonRead()){
		toogle=~ toogle;}
		//clearing status flag for this channel:
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		//lrd for test
//		ledsOn();
//		for (delay = 0; delay < 300000; delay++);
//		for (delay = 0; delay < 300000; delay++);
//		for (delay = 0; delay < 300000; delay++);
//		ledsOff();
		
	}
	
}
int8_t two_taps(void){
	return toogle&0x01;}

void toogle_clear(void){
toogle=0x00;}
