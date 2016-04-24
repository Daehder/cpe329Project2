/*
 Students: Mytch Johnson and Erik Miller
 Class: CPE 329-07 w/ Dr. Oliver
 Project 2: Create a function generator
 Date: 4/20/2016
 Revision: 1
 */

#include "WaveGen.h"
#include "arduinoUtil.h"

// Global Variables
uint8_t LUT_address = 0;
int num_samples;	// sets global number of samples
uint8_t overflow1 = 1;	// set overflow value frequency change
uint8_t overflow2 = 63;	// set overflow value for button checking

int main(void)
{
	num_samples = NUM_SAMPS;
	
   // initialize GPIO, interrupts, and timers
   GPIO_Initialization();
   
   // fill square, sawtooth, triangle, and sine wave LUTs
   initWaves();
   
   while (1){
	   if(check_buttons()==1)
			nextWave();
		else if(check_buttons()==2)
			change_freq();
   }
   return 0;
}

///////////////////////////////////ISR/////////////////////////////////////////

// ISR to increment through wave function LUTs and set frequency
ISR(TIMER0_COMPA_vect){
   Transmit_SPI_Master(nextWavePoint());
}

