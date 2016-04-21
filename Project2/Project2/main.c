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

#include <avr/io.h>
#include <util/delay.h>
#include <WaveGen.h>

// SPI helper functions
void Initialize_SPI_Master();
void Transmit_SPI_Master(int Data);
void send_to_DAC(uint16_t wave[], uint8_t frequency);

// general helper functions
void delay_ms(uint16_t delay);
void delay_us(uint16_t delay);
uint16_t volts_to_bits(double voltage);
void GPIO_Initialization();
uint8_t check_buttons();
uint16_t freq_to_delay(uint8_t freq);

int main(void)
{	
	GPIO_Initialization();
	
    while(1){
		// send square wave data to DAC
		while(!(check_buttons()));
		// send sawtooth wave data to DAC
		while(!(check_buttons()));
		// send triangle wave data to DAC
		while(!(check_buttons()));
		// send sine wave data to DAC
		while(!(check_buttons()));			
	}
    return 0;
}  

// sets up SPI system between ATmega328P and slave device(s)
void Initialize_SPI_Master(){     
    SPCR = (0<<SPIE) |         //No interrupts
    (1<<SPE) |             //SPI enabled
    (1<<MSTR) ;             //master
    SPSR = 0x00;             //clear flags
    PORTB = 1 << SS;          // make sure SS is high
}

// initializes GPIO I/O, SPI interface, and interrupts
void GPIO_Initialization(){
	DDRB |= (1<<MOSI) | (1<<SCK) | (1<<SS);		// make MOSI, SCK and SS outputs
	DDRB &= ~(1<<BTN1) | ~(1<<BTN1);			// set buttons as inputs
	PORTB |= (1<<BTN0) | (1<<BTN1);				// set internal pull-ups on buttons
	Initialize_SPI_Master();					// initialize SPI interface to DAC
	sei();										// enable global interrupts 
}

// returns a true bool if a buttons is pressed
uint8_t check_buttons(){
	if(!(PIND & (1<<BTN0)))			// return 1 if button at pin0 is pressed
		return 1;
	else if(!(PIND & (1<<BTN1)))	// return 2 if button at pin1 is pressed
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

