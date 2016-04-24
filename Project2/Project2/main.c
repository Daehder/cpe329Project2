/*
 Students: Mytch Johnson and Erik Miller
 Class: CPE 329-07 w/ Dr. Oliver
 Project 2: Create a function generator
 Date: 4/20/2016
 Revision: 1
 */

#include "WaveGen.h"
#include "arduinoUtil.h"

int btn0 = 0;
int btn1 = 0;
int btn2 = 0;

int was0Pressed = 0;
int was1Pressed = 0;
int was2Pressed = 0;

// Global Variables
uint8_t LUT_address = 0;
int num_samples;	// sets global number of samples
uint8_t overflow0 = 1;	// set overflow value frequency change
uint8_t overflow2 = 63;	// set overflow value for button checking

int main(void)
{
	num_samples = NUM_SAMPS;
	
   // initialize GPIO, interrupts, and timers
   GPIO_Initialization();
   
   // fill square, sawtooth, triangle, and sine wave LUTs
   initWaves();
   
   while (1){
	  // if(check_buttons()==1)
			//nextWave();
		//else if(check_buttons()==2)
		//	change_freq();
   }
   return 0;
}

///////////////////////////////////ISR/////////////////////////////////////////

// ISR to increment through wave function LUTs and set frequency
ISR(TIMER0_COMPA_vect){
   Transmit_SPI_Master(nextWavePoint());
   PORTD |= (1<<LED2);
 
}

ISR(TIMER2_COMPA_vect){
   if (!(PIND & 1 << BTN0))
      btn0++;
   else
      btn0 = was0Pressed = 0;
   
   if (!(PIND & 1 << BTN1))
      btn1++;
   else
      btn1 = was1Pressed = 0;
   
   if (!(PIND & 1 << BTN2))
      btn2++;
   else
      btn2 = was2Pressed = 0;
   
   if(btn0 >= DEBOUNCE) {
      if (!was0Pressed)
         nextWave();
      btn0 = 0;
      was0Pressed = 1;
   }
   
   if(btn1 >= DEBOUNCE) {
      if (!was1Pressed) {
         PORTD &= ~(1<<LED3);
         change_freq();
      }
      btn1 = 0;
      was1Pressed = 1;
   }
   
   if(btn2 >= DEBOUNCE) {
      if (!was2Pressed) {
         cycleDuty();
      }
	  PORTD |= (1<<LED3);
      btn2 = 0;
      was2Pressed = 1;
   }
  
}

