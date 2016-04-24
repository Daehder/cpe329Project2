/*
 Students: Mytch Johnson and Erik Miller
 Class: CPE 329-07 w/ Dr. Oliver
 Project 2: Create a function generator
 Date: 4/20/2016
 Revision: 1
 */

#include "WaveGen.h"
#include "arduinoUtil.h"

int num_samples;			// sets global number of samples
int overflow_100Hz = 200;		// set overflow value for 100Hz
int overflow_100hz = 40;		// set overflow value for 500Hz

// Global Variables
uint8_t LUT_address = 0;

int main(void)
{
	num_samples = NUM_SAMPS;
	
   // initialize GPIO, interrupts, and timers
   GPIO_Initialization();
   
   // fill square, sawtooth, triangle, and sine wave LUTs
   initWaves();
   
   while (1){
      PORTD |= (1<<LED2);
   }
   
   return 0;
}

///////////////////////////////////ISR/////////////////////////////////////////


// ISR to increment through wave function LUTs and set frequency
ISR(TIMER0_COMPA_vect){
   Transmit_SPI_Master(nextWavePoint());
//   LUT_address++;
//   
//   if (LUT_address>=num_samples)
//      LUT_address = 0;
}

