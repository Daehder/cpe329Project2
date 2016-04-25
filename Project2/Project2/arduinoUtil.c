/*
 * Students: Mytch Johnson and Erik Miller
 * Class: CPE 329-07 w/ Dr. Oliver
 * Project 2: Create a function generator
 * Date: 4/20/2016
 * Revision: 1
 */

#include "arduinoUtil.h"

#define LEVEL_100 4
#define LEVEL_200 3
#define LEVEL_300 2
#define LEVEL_400 1
#define LEVEL_500 0

#define LEVEL_DIVIDER 52

// Standard frequency values needed for overflow
#define OVERFLOW_100HZ 135
#define OVERFLOW_200HZ 67
#define OVERFLOW_300HZ 90
#define OVERFLOW_400HZ 67
#define OVERFLOW_500HZ 54

uint8_t sampleDivider = 1;

// sets up SPI system between ATmega328P and slave device(s)
void Initialize_ADC0()
{
   ADCSRA = 0x87;                   //Turn On ADC and set prescaler (CLK/128)
   ADCSRB = 0x00;                   //turn off autotrigger
   ADMUX = 0x00;                    //Set ADC channel ADC0
}

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
   OCR0A = OVERFLOW_100HZ;          // sets counter overflow
   TCCR0B = 0x02;                   // timer clk = system clk / 8 (2MHz)
   TIFR0 = 0x02;                    // Interrupt occurs at OCRF0A overflow
   TIMSK0 = 0x02;                   // OCRF0A overflow interrupt enabled
}

void initTimer2(){
   TCCR2A = 0x02;                   // timer CTC mode
   OCR2A = BTN_OVERFLOW;		      // sets counter overflow
   TCCR2B = 0x04;                   // timer clk = system clk / 256 (2MHz)
   TIFR2 = 0x02;                    // Interrupt occurs at OCRF2A overflow
   TIMSK2 = 0x02;                   // OCRF0A overflow interrupt enabled
}

// initializes GPIO I/O, SPI interface, and interrupts
void GPIO_Initialization(){
   DDRB |= (1<<MOSI) | (1<<SCK)
         | (1<<SS);                 // make MOSI, SCK and SS outputs
   DDRD &= ~(1<<BTN1) | ~(1<<BTN1)
         | ~(1<<SW1);               // set buttons & switches as inputs
   DDRD |= (1<<LED2) | (1<<LED3);	// debug LEDs are output
   PORTD |= (1<<BTN0) | (1<<BTN1);               // set internal pull-ups
   
   Initialize_SPI_Master();			// initialize SPI to DAC
   initTimer0();
   initTimer2();
   Initialize_ADC0();
}

uint8_t check_switch(){
   if(PIND & 1<<SW1){
	   PORTD &= ~(1<<LED3);
	   return 1;
   }
	else{
		PORTD |= (1<<LED3);
		return 0;
	}
}

uint16_t check_voltage(){
   ADCSRA = 0xC7;                   // start conversion
   _delay_us(260);                  // ensure max sampling rate not exceeded
   uint16_t voltage = (ADC & 0x3FF)/4;  // read 10 bit voltage and convert to 8 bit
   
   if(voltage < 2)
      return 1;
   else
      return voltage;
}

// sends data to DAC over SPI data port
void Transmit_SPI_Master(uint8_t Data) {
   PORTB &= ~(1 << SS);					// Assert slave select (active low)
   SPDR = ((Data >> 8) & 0xF);      // Put top 4 bits onto SPDR
   SPDR |= 0x70;                    // Attach configuration bits onto MSB
   
   while (!(SPSR & (1<<SPIF)))
      ;
   
   SPDR = 0xFF & Data;              // Put the remaining 8 bits onto SPDR
   
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
   double bits = ((voltage / 5.0) * 4095);
   
   if (bits > 4095)
      bits =  4095;
   return bits;                     // return 12bit equivalent for DAC
}

// Expects a value between 0 and 255, and sets the overflow to the corresponding
//  level for incremental frequency mode
void setFreq(uint8_t level) {
   level /= LEVEL_DIVIDER;
   
   if (level == LEVEL_100){
      sampleDivider = 1;
      OCR0A = OVERFLOW_100HZ;
   }
   else if (level == LEVEL_200) {
      sampleDivider = 1;
      OCR0A = OVERFLOW_200HZ;
   }
   else if (level == LEVEL_300) {
      sampleDivider = 2;
      OCR0A = OVERFLOW_300HZ;
   }
   else if (level == LEVEL_400) {
      sampleDivider = 2;
      OCR0A = OVERFLOW_400HZ;
   }
   else{
      sampleDivider = 2;
      OCR0A = OVERFLOW_500HZ;
   }
}

// Expects a value between 0 and 255, and sets the overflow to that value for
//  variable frequency mode
void sweepFreq(uint8_t level) {
   sampleDivider = 2;
   OCR0A = level;
}
