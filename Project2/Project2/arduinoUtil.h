//
//  arduinoUtil.h
//  cpe329project2
//
//  Created by Erik Miller on 4/22/16.
//
//

#ifndef arduinoUtil_h
#define arduinoUtil_h

#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

extern int num_samples =  50;			// sets global number of samples
extern int overflow_100Hz = 200;		// set overflow value for 100Hz
extern int overflow_100hz = 40;		// set overflow value for 500Hz

// initialization helper functions
void Initialize_SPI_Master();
void Transmit_SPI_Master(int Data);
void GPIO_Initialization();
void initTimer0();
void genorate_LUTs();

// general helper functions
void delay_us(uint16_t delay);
void delay_ms(uint16_t delay);
uint8_t check_buttons();

// Program 2 specific functions
uint16_t volts_to_bits(double voltage);

#endif /* arduinoUtil_h */
