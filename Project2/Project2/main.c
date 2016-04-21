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
void Initialize_SPI_Master(void);
void Transmit_SPI_Master(int Data);

// general helper functions
void delay_ms(uint16_t delay);
void delay_us(uint16_t delay);
uint16_t volts_to_bits(double voltage);

int main(void)
{
    //int count = 0;
    //int delta = 1;
    //int tri_top = 0xFFF; // top is at 5V
    //int tri_bottom = 0x000; // bottom is 0V
    DDRB = 1<<MOSI | 1<<SCK | 1<<SS;    // make MOSI, SCK and SS outputs
    Initialize_SPI_Master();
    while(1)
    {
/*
        Transmit_SPI_Master(count);     //0xAAA / 0xFFF = 2730 / 4096 * 5V (ref) = ~ 3.33V 
           _delay_us(100);
        
        if (count == tri_top)
            delta = -1;
        else if (count == tri_bottom)
            delta = 1;
            
        count += delta;
*/

    }  // end while
    return 0;
}  // end main

void Initialize_SPI_Master(void)
{      
    SPCR = (0<<SPIE) |         //No interrupts
    (1<<SPE) |             //SPI enabled
    (1<<MSTR) ;             //master
    SPSR = 0x00;             //clear flags
    PORTB = 1 << SS;          // make sure SS is high

}

void Transmit_SPI_Master(int Data) { 
PORTB &= ~(1 << SS);               //Assert slave select (active low)
SPDR = ((Data >> 8) & 0xF) | 0x70;       //Attach configuration Bits onto MSB 
while (!(SPSR & (1<<SPIF))); 
SPDR = 0xFF & Data; 
while (!(SPSR & (1<<SPIF))); 
PORTB |= 1 << SS;               //Turn off slave select 
}

void delay_us(uint16_t delay){
	while(delay){
		_delay_us(1);
		delay--;
	}
}

void delay_ms(uint16_t delay){
	while(delay){
		_delay_ms(1);
		delay--;
	}
}

