#ifndef i2c_h
#define i2c_h
#include "MKL05Z4.h"   /* Device header */

void i2c_init(void);
void i2c_deinit(void);
uint8_t receive(void);
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);
uint8_t i2c_repeated_read(uint8_t isLastRead);
void i2c_read_setup(uint8_t dev, uint8_t address);
void i2c_wait(void);
void i2c_start(void);
void accel_interupt(void);
#endif
