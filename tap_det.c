#include "tap_det.h"
void tap_det(void){
volatile int delay;
volatile uint8_t sequence;
ledBlueOn ();
	for(delay=0; delay<300000; delay++);
	for(delay=0; delay<300000; delay++);
	ledsOff();
	sequence=0;
	while(1){
	if(!buttonRead()){
		sequence++;
		for(delay=0; delay<300000; delay++);
		for(delay=0; delay<300000; delay++);
		for(delay=0; delay<300000; delay++);
		for(delay=0; delay<300000; delay++);
	}
	if(two_taps()){
		sequence=0;
		toogle_clear();
	}
	switch(sequence)
	{
		case 1:
			ledsOn();
		break;
		
		case 2:
			ledRedOn();
		break;
		
		case 3:
				ledGreenOn();
		break;
		
			case 4:
				ledBlueOn();
		break;
			
			case 5:
				ledYellowOn();
		break;
				case 6:
				ledPurpleOn();
		break;
				case 7:
				ledUnknownOn();
		break;
				
				case 8:
				sequence=0;
		break;
			
			default:
				ledsOff();
	}
	}}
