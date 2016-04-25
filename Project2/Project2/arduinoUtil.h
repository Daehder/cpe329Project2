//
//  arduinoUtil.h
//  cpe329project2
//
//  Created by Erik Miller on 4/22/16.
//
//

#ifndef arduinoUtil_h

#define arduinoUtil_h

// Hardware CPU clock speed
#define F_CPU 16000000              //define internal CLK speed

// SPI pin locations
#define MOSI 3                      // PB pin 3
#define SCK  5                      // PB pin 5
#define SS   2                      // PB pin 2

// Debug LED pin Locations
#define LED2 2
#define LED3 3

// Input pin locations
#define BTN0 7                      // Button 0
#define BTN1 6                      // Button 1
#define BTN2 5						// Button 2
#define SW1 4						// switch 0
#define POT0 0						// Potentiometer 

// non changing values
#define NUM_SAMPS 146
#define DEBOUNCE 10

// Standard frequency values needed for overflow
#define OVERFLOW_100HZ 135 //done
#define OVERFLOW_200HZ 67 //done
#define OVERFLOW_300HZ 90 // done
#define OVERFLOW_400HZ 67 //done
#define OVERFLOW_500HZ 10 //done

#define LEVEL_100 0
#define LEVEL_200 1
#define LEVEL_300 2
#define LEVEL_400 3
#define LEVEL_500 4

#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

extern int num_samples;			// sets global number of samples
extern int sampleDivider;
extern uint8_t overflow0;
extern uint8_t overflow2;  
extern uint16_t voltage; 

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
uint8_t check_switch();
uint16_t check_voltage();

// Program 2 specific functions
uint16_t volts_to_bits(double voltage);
void cycleFreq();
void setFreq(int level);

#endif /* arduinoUtil_h */
