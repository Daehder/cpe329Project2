/*
Students: Mytch Johnson and Erik Miller
Class: CPE 329-07 w/ Dr. Oliver
Project 2: Create a function generator
Date: 4/20/2016
Revision: 1
*/

int num_samples =  50;			// sets global number of samples
int overflow_100Hz = 200;		// set overflow value for 100Hz
int overflow_100hz = 40;		// set overflow value for 500Hz

#include "WaveGen.h"
#include "arduinoUtil.h"

// Global Variables
uint8_t LUT_address = 0;
uint16_t square_wave[NUM_SAMPLES];
uint16_t sawtooth_wave[NUM_SAMPLES];
uint16_t triangle_wave[NUM_SAMPLES];
uint16_t sin_wave[NUM_SAMPLES];

int main(void)
{	
	// initialize GPIO, interupts, and timers
	GPIO_Initialization();
	
	// fill square, sawtooth, triangle, and sine wave LUTs
	genorate_LUTs();
	
	// stay here forever
    while(1){
		// send square wave data to DAC
		Transmit_SPI_Master(square_wave[LUT_address]);
		while(!(check_buttons()));
		// send sawtooth wave data to DAC
		Transmit_SPI_Master(sawtooth_wave[LUT_address]);
		while(!(check_buttons()));
		// send triangle wave data to DAC
		//Transmit_SPI_Master([LUT_address]);
		while(!(check_buttons()));
		// send sine wave data to DAC
		//Transmit_SPI_Master(LUT_address[LUT_address]);
		while(!(check_buttons()));			
	}
    return 0;
}

// fills all the lookup tables for every wave
void genorate_LUTs(){
	
}

///////////////////////////////////ISR/////////////////////////////////////////

// ISR to incrament through wave function LUTs and set frequency 
ISR(TIMER0_COMPA_vect){
	static uint8_t ISR_repeat=0;
	
	if((ISR_repeat = !ISR_repeat))
		// increment wave LUT value
		LUT_address++;
}
