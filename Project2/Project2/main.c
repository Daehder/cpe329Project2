/*
 Students: Mytch Johnson and Erik Miller
 Class: CPE 329-07 w/ Dr. Oliver
 Project 2: Create a function generator
 Date: 4/20/2016
 Revision: 1
 */

// I hope this is where we want to be

int num_samples =  150;			// sets global number of samples
int overflow_100Hz = 200;		// set overflow value for 100Hz
int overflow_100hz = 40;		// set overflow value for 500Hz

#include "WaveGen.h"
#include "arduinoUtil.h"

// Global Variables
uint8_t LUT_address = 0;

int main(void)
{
   // initialize GPIO, interrupts, and timers
   GPIO_Initialization();
   
   // fill square, sawtooth, triangle, and sine wave LUTs
   genorate_LUTs();
   
   while (1){
      if(check_buttons())
         PORTB |= (1<<LED12);
      else
         PORTB &= ~(1<<LED12);
   }
   
   return 0;
}

// fills all the lookup tables for every wave
void genorate_LUTs(){
   make_square_LUT();		// fill square wave LUT
   make_sawtooth_LUT();	// fill sawtooth wave LUT
   make_triangle_LUT();	// fill triangle wave LUT
   make_sin_LUT();			// fill sine wave LUT
   sei();
}

///////////////////////////////////ISR/////////////////////////////////////////


// ISR to increment through wave function LUTs and set frequency
ISR(TIMER0_COMPA_vect){
   Transmit_SPI_Master(SinWave[LUT_address]);
   LUT_address++;
   
   if (LUT_address>=NUM_SAMPLES)
      LUT_address = 0;
}

