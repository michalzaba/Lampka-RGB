#include "MKL05Z4.h"                    	/* Device header */
#include "i2c.h"													/* Leds functions for FRDM46-KL46 */
#include <stdio.h>
#include <string.h>


#define I2C_M_START I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART I2C0->C1 |= I2C_C1_RSTA_MASK
#define I2C_TRAN I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC I2C0->C1 &= ~I2C_C1_TX_MASK
#define TRANS_COMP while(!(I2C0->S & 0x80))
#define I2C_WAIT i2c_wait();
#define MMA_ADDR (0x1d<<1)
#define NACK I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK I2C0->C1 &= ~I2C_C1_TXAK_MASK
#define BUSY_ACK while(I2C0->S & 0x01)
void i2c_start()
{
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
}
 void i2c_init()
{
	unsigned char  temp;
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
  SIM->SCGC5 |=  (SIM_SCGC5_PORTB_MASK);

	PORTB->PCR[3] = PORT_PCR_MUX(2);  
	PORTB->PCR[4] = PORT_PCR_MUX(2);  
	//	I2C baud rate set to 93,75k baud
 	I2C0->F = (0x14);
		
	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);
	I2C0->C1 |= (I2C_C1_IICIE_MASK);
	
	// Select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
	temp=i2c_read_byte(0x1d<<1, 0x2a);
	i2c_write_byte(MMA_ADDR, 0x2a,temp|0x01);
}  
void i2c_deinit()
{    I2C0->C1 = 0x00;
    SIM->SCGC4 &= ~SIM_SCGC4_I2C0_MASK;
}
void accel_interupt(void)
{	
	i2c_init();
	i2c_write_byte(0x1d<<1,0x2a,0x08);
	i2c_write_byte(0x1d<<1,0x21,0x15);//enable single pulse xyz
	i2c_write_byte(0x1d<<1,0x23,0x10);//x treshold =1.575g
	i2c_write_byte(0x1d<<1,0x24,0x10);//y treshold =1.575g
	i2c_write_byte(0x1d<<1,0x25,0x10);//z treshold =1.575g
	i2c_write_byte(0x1d<<1,0x26,0x50);//Data Rate 400 Hz, time step is 0.625 ms
	i2c_write_byte(0x1d<<1,0x27,0xf8);//set Latency Time to 300 ms
	i2c_write_byte(0x1d<<1,0x2d,0x08);///Enable Pulse Interrupt Block 
	i2c_write_byte(0x1d<<1,0x2e,0xf7);///Enable Pulse Interrupt Block hw pin
	i2c_init();
}
void i2c_wait(void) {
	while((I2C0->S & I2C_S_IICIF_MASK)==0) {
	} \
  I2C0->S |=I2C_S_IICIF_MASK;
}


//send device and register addresses
#pragma no_inline 
void i2c_read_setup(uint8_t dev, uint8_t address)
{
	i2c_start();							/*set to transmit mode */
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT							/*wait for completion */
	I2C_TRAN;
	I2C0->D =  address;		/*send read address	*/
	I2C_WAIT							/*wait for completion */	
	I2C_M_RSTART;				   /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	I2C_WAIT							 /*wait for completion */
	I2C_REC;						   /*set to receive mode */

}

//read a byte and ack/nack as appropriate
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;
	if(isLastRead)	{
		NACK;								/*set NACK after read	*/
	} else	{
		ACK;								/*ACK after read	*/
	}
	
	data = I2C0->D;				/*dummy read	*/
	I2C_WAIT							/*wait for completion */
	
	if(isLastRead)	{
		I2C_M_STOP;					/*send stop	*/
	}
	data = I2C0->D;				/*read data	*/

	return  data;					
}




//7bit addressing reads a byte from dev:address

uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;
	i2c_start();							/*set to transmit mode */
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT							/*wait for completion */
	
	I2C0->D =  address;		/*send read address	*/
	I2C_WAIT							/*wait for completion */
		
	I2C_M_RSTART;				   /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	I2C_WAIT							 /*wait for completion */
	
	I2C_REC;						   /*set to recieve mode */
	NACK;									 /*set NACK after read	*/
	
	data = I2C0->D;					/*dummy read	*/
	I2C_WAIT								/*wait for completion */
	
	I2C_M_STOP;							/*send stop	*/
	data = I2C0->D;					/*read data	*/

	return data;
}



//using 7bit addressing writes a byte data to dev:address
//#pragma no_inline 
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{volatile int delay;
	
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT						  /*wait for ack */
	
	I2C0->D =  address;		/*send write address	*/
	I2C_WAIT
		
	I2C0->D = data;				/*send data	*/
	I2C_WAIT
	I2C_M_STOP;
	
}
