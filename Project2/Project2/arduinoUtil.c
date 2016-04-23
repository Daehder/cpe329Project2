//
//  arduinoUtil.c
//  cpe329project2
//
//  Created by Erik Miller on 4/22/16.
//
//

#include "arduinoUtil.h"

#define MOSI 3                      // PB pin 3
#define SCK  5                      // PB pin 5
#define SS   2                      // PB pin 2
#define BTN0 0                      // Button 0 us at pin 0
#define BTN1 1                      // Button 1 is at pin 1
#define ANALOGIN0 0                 // analog input at pin A0

// sets up SPI system between ATmega328P and slave device(s)
void Initialize_SPI_Master(){
   SPCR = (0<<SPIE) |               // No interrupts
   (1<<SPE) |                       // SPI enabled
   (1<<MSTR) ;                      // master
   SPSR = 0x00;                     // clear flags
   PORTB = 1 << SS;                 // make sure SS is high
}

// Initializing timer 0 (change OCR0A for freq change!!!!!)
void initTimer0(){
   TCCR0A = 0x02;                   // timer CTC mode
   OCR0A = overflow_100Hz;          // sets counter overflow to 250
   TCCR0B = 0x02;                   // timer clk = system clk / 8 (2MHz)
   TIFR0 = 0x02;                    // Interrupt occurs at OCRF0A overflow
   TIMSK0 = 0x02;                   // OCRF0A overflow interrupt enabled
}


// initializes GPIO I/O, SPI interface, and interrupts
void GPIO_Initialization(){
   DDRB |= (1<<MOSI) | (1<<SCK) | (1<<SS);	// make MOSI, SCK and SS outputs
   DDRB &= ~(1<<BTN1) | ~(1<<BTN1);	// set buttons as inputs
   PORTB |= (1<<BTN0) | (1<<BTN1);	// set internal pull-ups
   Initialize_SPI_Master();			// initialize SPI to DAC
   initTimer0();							// initialize timer0
   sei();									// enable interrupts
}

// returns a true bool if a buttons is pressed
uint8_t check_buttons(){
   if (!(PIND & (1<<BTN0)))          // return 1 if button at pin0 is pressed
      return 1;
   else if(!(PIND & (1<<BTN1)))     // return 2 if button at pin1 is pressed
      return 2;
   else
      return 0;                     // return 0 if no buttons are pressed
}

// sends data to DAC over SPI data port
void Transmit_SPI_Master(int Data) {
   PORTB &= ~(1 << SS);					//Assert slave select (active low)
   SPDR = ((Data >> 8) & 0xF) | 0x70;		//Attach configuration Bits onto MSB
   while (!(SPSR & (1<<SPIF)))
      ;
   SPDR = 0xFF & Data;
   while (!(SPSR & (1<<SPIF)))
      ;
   PORTB |= 1 << SS;						//Turn off slave select
}

// allows delays with variable input for us range
void delay_us(uint16_t delay){
   while (delay--)
      _delay_us(1);
   
}

// allows delays with variable input for ms range
void delay_ms(uint16_t delay){
   while (delay--)
      _delay_ms(1);

}

// converts input voltage between 0-5V to 12bit number for DAC
uint16_t volts_to_bits(double voltage){
   double bits = ((voltage/5.0)*4095);
   if(bits>4095)
		return 4095;
	else
	 return bits;                  // return 12bit equivalent for DAC
}