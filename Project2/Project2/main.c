/*
Students: Mytch Johnson and Erik Miller
Class: CPE 329-07 w/ Dr. Oliver
Project 2: Create a function generator
Date: 4/20/2016
Revision: 1
*/

#define F_CPU 16000000          //define internal CLK speed
#define MOSI 3                  // PB pin 3
#define SCK  5                  // PB pin 5
#define SS   2                  // PB pin 2
#define BTN0 5					// Button 0 us at pin 0
#define BTN1 6					// Button 1 is at pin 1
#define ANALOGIN0 0				// analog input at pin A0
#define LED12 7					// On-board LED at PB4 pin12
#define NUM_SAMPLES 200			// sets global number of samples
#define OVERFLOW_100Hz 100		// set overflow value for 100Hz
#define OVERFLOW_500Hz 20		// set overflow value for 500Hz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "WaveGen.h"

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

// Global Variables
uint8_t LUT_address = 0;

int main(void)
{	
	// initialize GPIO, interrupts, and timers
	GPIO_Initialization();
	
	// fill square, sawtooth, triangle, and sine wave LUTs
	genorate_LUTs();

	while(1){
		if(check_buttons())
			PORTB |= (1<<LED12);
		else 
			PORTB &= ~(1<<LED12);
	}
	
    return 0;
}  

// sets up SPI system between ATmega328P and slave device(s)
void Initialize_SPI_Master(){     
    SPCR = (0<<SPIE) |		//No interrupts
    (1<<SPE) |				//SPI enabled
    (1<<MSTR) ;             //master
    SPSR = 0x00;			//clear flags
    PORTB = 1 << SS;		// make sure SS is high
}

// Initializing timer 0 (change OCR0A for freq change!!!!!)
void initTimer0(){
	TCCR0A = 0x02;			// timer CTC mode
	OCR0A = OVERFLOW_100Hz;	// sets counter overflow to 250
	TCCR0B = 0x02;			// timer clk = system clk / 8 (2MHz)
	TIFR0 = 0x02;			// Interrupt occurs at OCRF0A overflow 
	TIMSK0 = 0x02;			// OCRF0A overflow interrupt enabled
}


// initializes GPIO I/O, SPI interface, and interrupts (working)
void GPIO_Initialization(){
	DDRB |= (1<<MOSI) |						// make MOSI, SCK and SS outputs
			(1<<SCK)  | 
			(1<<SS);
	DDRD |= (1<<LED12);						// led at PB4 is output
	DDRD &= ~(1<<BTN0) |					// buttons 0 and 1 are inputs
			~(1<<BTN1);
	PORTD |= (1<<BTN0) | (1<<BTN1);			// set internal pull-ups on btns
	Initialize_SPI_Master();				// initialize SPI to DAC
	initTimer0();							// initialize timer0
}

// fills all the lookup tables for every wave
void genorate_LUTs(){
	make_square_LUT();		// fill square wave LUT
	make_sawtooth_LUT();	// fill sawtooth wave LUT
	make_triangle_LUT();	// fill triangle wave LUT
	make_sin_LUT();			// fill sine wave LUT
	sei();
}

// returns a true bool if a buttons is pressed
uint8_t check_buttons(){
	if(!(PINB & (1<<BTN0)))			// return 1 if button at pin0 is pressed
		return 1;
	else if(!(PINB & (1<<BTN1)))	// return 2 if button at pin1 is pressed
		return 2;
	else
		return 0;					// return 0 if no buttons are pressed
}

// sends data to DAC over SPI data port
void Transmit_SPI_Master(int Data) { 
PORTB &= ~(1 << SS);					//Assert slave select (active low)
SPDR = ((Data >> 8) & 0xF) | 0x70;		//Attach configuration Bits onto MSB 
while (!(SPSR & (1<<SPIF))); 
SPDR = 0xFF & Data; 
while (!(SPSR & (1<<SPIF))); 
PORTB |= 1 << SS;						//Turn off slave select 
}

// allows delays with variable input for us range
void delay_us(uint16_t delay){
	while(delay){
		_delay_us(1);
		delay--;
	}
}

// allows delays with variable input for ms range
void delay_ms(uint16_t delay){
	while(delay){
		_delay_ms(1);
		delay--;
	}
}

// converts input voltage between 0-5V to 12bit number for DAC
uint16_t volts_to_bits(double voltage){
	double bits = ((voltage/5.0)*4095);
	if(bits > 4095)						// if given above 5V return 5V
		return 4095;
	else
		return bits;					// return 12bit equivalent for DAC
}

///////////////////////////////////ISR/////////////////////////////////////////


// ISR to increment through wave function LUTs and set frequency 
ISR(TIMER0_COMPA_vect){
	Transmit_SPI_Master(SinWave[LUT_address]);
	LUT_address++;
	
	if(LUT_address>NUM_SAMPLES-1)
		LUT_address = 0;
}

