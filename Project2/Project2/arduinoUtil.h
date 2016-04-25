/*
 * Students: Mytch Johnson and Erik Miller
 * Class: CPE 329-07 w/ Dr. Oliver
 * Project 2: Create a function generator
 * Date: 4/20/2016
 * Revision: 1
 */

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
#define BTN1 5                      // Button 1
#define SW1 4                       // switch 0
#define POT0 0                      // Potentiometer 

// non changing values
#define NUM_SAMPLES 146
#define DEBOUNCE 10


#define BTN_OVERFLOW 63

#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

extern uint8_t sampleDivider;

// initialization helper functions
void Initialize_SPI_Master();
void Transmit_SPI_Master(uint8_t);
void GPIO_Initialization();
void initTimer0();
void genorate_LUTs();
void change_freq();

// general helper functions
void delay_us(uint16_t);
void delay_ms(uint16_t);
uint8_t check_buttons();
uint8_t check_switch();
uint16_t check_voltage();

// Program 2 specific functions
uint16_t volts_to_bits(double);
void setFreq(uint8_t);
void sweepFreq(uint8_t);

#endif /* arduinoUtil_h */
