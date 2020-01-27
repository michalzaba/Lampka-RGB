#include "led.h" //Declarations
#define IN1_PIN 10
const uint32_t  red_mask=1UL<<8 ;//Red led is Port D bit 5
const uint32_t green_mask=1UL<<9;
const uint32_t blue_mask=1UL<<10;//Green led is Port C bit 5/


/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void ledsInitialize(void) {
volatile int delay;
	
//Initialize registers	
  SIM->SCGC5 |=  (SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTA_MASK );      /* Enable Clock to Port B & A*/ 
  PORTB->PCR[8] = PORT_PCR_MUX(1)|0x40u;                       //ledsInitialize's gpio mux 
  PORTB->PCR[9] = PORT_PCR_MUX(1)|0x40u;                      
	PORTB->PCR[10] = PORT_PCR_MUX(1)|0x40u;
	PORTA->PCR[10]=PORT_PCR_MUX(1) | 0x2u| 0x1u; //port A10 input pullup
	PTB->PSOR = green_mask|red_mask|blue_mask;        /* All LED's off */
	PTB->PDDR = green_mask|red_mask|blue_mask	;        /* enable LED RGB as Output */
	PTA->PDDR =~(1UL<<IN1_PIN);

//Welcome sequence
	for(delay=0; delay<300000; delay++);
	PTB->PCOR = green_mask	;        /* switch Green LED on */
	for(delay=0; delay<300000; delay++);
	PTB->PSOR = green_mask	;        /* switch Green LED off */
	for(delay=0; delay<300000; delay++);
		PTB->PCOR = green_mask	;  /* switch Green LED on */
		PTB->PCOR = red_mask	;/* switch Red LED on */
	for(delay=0; delay<300000; delay++);
	PTB->PCOR=red_mask;          	/* switch Red LED on */
	PTB->PSOR=green_mask;          /* switch Green LED off */
	PTB->PSOR=blue_mask;						/* switch Blue LED off  */
}

/*----------------------------------------------------------------------------
  Function that turns on Red LED
 *----------------------------------------------------------------------------*/
void ledRedOn (void) {
	FPTB->PCOR=red_mask;          	/* switch Red LED on */
	FPTB->PSOR=green_mask;          /* switch Green LED off */
	FPTB->PSOR=blue_mask;						/* switch Blue LED off  */
}

/*----------------------------------------------------------------------------
  Function that turns on Green LED
 *----------------------------------------------------------------------------*/
void ledGreenOn (void) {
	FPTB->PCOR=green_mask;       		/* switch Green LED on */
	FPTB->PSOR=red_mask;          	/* switch Red LED off  */
	FPTB->PSOR=blue_mask;						/* switch Blue LED off  */
}

void ledBlueOn (void) {
	FPTB->PSOR=green_mask;       		/* switch Green LED off */
	FPTB->PSOR=red_mask;          	/* switch Red LED off  */
	FPTB->PCOR=blue_mask;						/* switch Blue LED off  */
}
void ledYellowOn (void) {
	FPTB->PCOR=green_mask;       		/* switch Green LED on */
	FPTB->PCOR=red_mask;          	/* switch Red LED on */
	FPTB->PSOR=blue_mask;						/* switch Blue LED off  */
}
void ledPurpleOn (void) {
	FPTB->PSOR=green_mask;       		/* switch Green LED off */
	FPTB->PCOR=red_mask;          	/* switch Red LED on */
	FPTB->PCOR=blue_mask;						/* switch Blue LED on  */
}
void ledUnknownOn (void) {
	FPTB->PCOR=green_mask;       		/* switch Green LED on */
	FPTB->PSOR=red_mask;          	/* switch Red LED off */
	FPTB->PCOR=blue_mask;						/* switch Blue LED on  */
}

/*----------------------------------------------------------------------------
  Function that turns all LEDs off
 *----------------------------------------------------------------------------*/
void ledsOff (void) {
		FPTB->PSOR=red_mask;          /* ALL OFF */
	  FPTB->PSOR=green_mask;        
		FPTB->PSOR=blue_mask;
}

/*----------------------------------------------------------------------------
  Function that turns all LEDs on
 *----------------------------------------------------------------------------*/
void ledsOn (void) {
		FPTB->PCOR=red_mask;      	/* ALL ON */
	  FPTB->PCOR=green_mask;     	
		FPTB->PCOR=blue_mask;
}

int32_t buttonRead(){
	return FPTA->PDIR & (1UL<<IN1_PIN);						/* Get port data input register (PDIR) */
}	
