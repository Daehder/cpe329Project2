//
//  arduinoUtil.h
//  cpe329project2
//
//  Created by Erik Miller on 4/22/16.
//
//

#ifndef arduinoUtil_h

#define arduinoUtil_h

#define F_CPU 16000000              //define internal CLK speed
#define LED2 2
#define LED3 3
#define NUM_SAMPS 150

#define BTN0 7                      // Button 0
#define BTN1 6                      // Button 1
#define BTN2 5						// Button 2

#define DEBOUNCE 10

#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

extern int num_samples;			// sets global number of samples
extern int sampleDivider;
extern uint8_t overflow1;
extern uint8_t overflow2;  

// initialization helper functions
void Initialize_SPI_Master();
void Transmit_SPI_Master(int Data);
void GPIO_Initialization();
void initTimer0();
void genorate_LUTs();
void change_freq();

// general helper functions
void delay_us(uint16_t delay);
void delay_ms(uint16_t delay);
uint8_t check_buttons();

// Program 2 specific functions
uint16_t volts_to_bits(double voltage);

#endif /* arduinoUtil_h */
