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
uint8_t btn0 = 0;
uint8_t btn1 = 0;

uint8_t was0Pressed = 0;
uint8_t was1Pressed = 0;

uint16_t voltage = 0;

int main(void)
{
   GPIO_Initialization();  // initialize GPIO, interrupts, and timers
   initWaves();            // fill square, sawtooth, triangle, and sine LUTs
   
   sei();									// enable interrupts
   
   PORTD |= (1<<LED2);     // indicator LED that system is ready
   
   // Stuck here forever
   while (1){
      if(check_voltage() <= 51) {
			PORTD |= (1<<LED2);
         setFreq(LEVEL_500);
      }
      else if( check_voltage() > 51 && check_voltage() <= 102) {
			PORTD &= ~(1<<LED2);
         setFreq(LEVEL_400);
      }
      else if( check_voltage() > 102 && check_voltage() <= 153) {
			PORTD |= (1<<LED2);
         setFreq(LEVEL_300);
      }
      else if( check_voltage() > 153 && check_voltage() <= 204) {
         PORTD &= ~(1<<LED2);
         setFreq(LEVEL_200);
      }
      else if( check_voltage() > 204 && check_voltage() <= 255) {
         PORTD |= (1<<LED2);
         setFreq(LEVEL_100);
      }
		else
			PORTD &= ~(1<<LED3);
			
		if(check_switch())
			PORTD |= (1<<LED3);
		else 
			PORTD &= ~(1<<LED3);	
   }
   
   return 0;
}

///////////////////////////////////ISR/////////////////////////////////////////

// ISR to increment through wave function LUTs and set frequency
ISR(TIMER0_COMPA_vect){
   Transmit_SPI_Master(nextWavePoint());
}

// ISR to check the buttons
ISR(TIMER2_COMPA_vect){
   if (!(PIND & 1 << BTN0))
      btn0++;
   else
      btn0 = was0Pressed = 0;
   
   if (!(PIND & 1 << BTN1))
      btn1++;
   else
      btn1 = was1Pressed = 0;
   
   if(btn0 >= DEBOUNCE) {
      if (!was0Pressed)
         nextWave();
      btn0 = 0;
      was0Pressed = 1;
   }
   
   if(btn1 >= DEBOUNCE) {
      if (!was1Pressed) {
         cycleDuty();
      }
      //PORTD |= (1<<LED3);
      btn1 = 0;
      was1Pressed = 1;
   }
   
}
