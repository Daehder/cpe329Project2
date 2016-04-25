//
//  arduinoUtil.c
//  cpe329project2
//
//  Created by Erik Miller on 4/22/16.
//
//

#include "arduinoUtil.h"

int sampleDivider = 1;

// sets up SPI system between ATmega328P and slave device(s)
void Initialize_ADC0(void)
{
   ADCSRA = 0x87;	//Turn On ADC and set prescaler (CLK/128)
   ADCSRB = 0x00;	//turn off autotrigger
   ADMUX = 0x00;    	//Set ADC channel ADC0
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
   OCR0A = overflow0;          // sets counter overflow
   TCCR0B = 0x02;                   // timer clk = system clk / 8 (2MHz)
   TIFR0 = 0x02;                    // Interrupt occurs at OCRF0A overflow
   TIMSK0 = 0x02;                   // OCRF0A overflow interrupt enabled
}

void initTimer2(){
   TCCR2A = 0x02;                   // timer CTC mode
   OCR2A = overflow2;		        // sets counter overflow
   TCCR2B = 0x04;                   // timer clk = system clk / 256 (2MHz)
   TIFR2 = 0x02;                    // Interrupt occurs at OCRF2A overflow
   TIMSK2 = 0x02;                   // OCRF0A overflow interrupt enabled
}


// initializes GPIO I/O, SPI interface, and interrupts
void GPIO_Initialization(){
   DDRB |= (1<<MOSI) | (1<<SCK) | (1<<SS);	// make MOSI, SCK and SS outputs
   DDRD &= ~(1<<BTN1) | ~(1<<BTN1) | ~(1<<BTN2) | ~(1<<SW1);	// set buttons & switches as inputs
   DDRD |= (1<<LED2) | (1<<LED3);				// debug LEDs are output
   PORTD |= (1<<BTN0) | (1<<BTN1) | (1<<BTN2);	// set internal pull-ups
   Initialize_SPI_Master();			// initialize SPI to DAC
   initTimer0();							// initialize timer0
   initTimer2();
   Initialize_ADC0();
   sei();									// enable interrupts
}

char debounceBtn(int btnLoc) {
   char ones=0, zeroes=0, i;
   
   for(i=0;i<9;i++){
      if(!(PIND & 1 << btnLoc)) // read pin == 1
         ones++;
      else // read pin == 0
         zeroes++;
      
      _delay_us(DEBOUNCE);
   }
   return ones > zeroes;
}

// returns a true bool if a buttons is pressed
uint8_t check_buttons(){
   static int wasPressed = 0;
   int btnPressed = 0;
   
   if (debounceBtn(BTN0))           // return 1 if button at pin7 is pressed
      btnPressed = 1;
   else if(debounceBtn(BTN1))       // return 2 if button at pin6 is pressed
      btnPressed = 2;
   else if(debounceBtn(BTN2))
      btnPressed = 3;               // return 3 if buttons at pin5 pressed
   
   if(btnPressed == wasPressed)
      return 0;
   
   wasPressed = btnPressed;
   
   return btnPressed;
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
   ADCSRA = 0xC7;        		// start conversion
   _delay_us(260);				// ensure max sampling rate not exceeded
   voltage = (ADC & 0x3FF)/4;  // read 10 bit voltage and convert to 8 bit (255)
   if(voltage < 2)
      return 1;
   else
      return voltage;
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

void change_freq(){
   //	overflow -= 50;
   //	if(overflow<50)
   //		overflow = 200;
   //
   //	OCR0A = overflow;          // sets counter overflow to 250
   sampleDivider++;
   sampleDivider %= 3;
}

void cycleFreq() {
   
}

void setFreq(int level) {
   if (level == LEVEL_100){
      sampleDivider = 1;
      overflow0 = OVERFLOW_100HZ;
   }
   else if (level == LEVEL_200) {
      sampleDivider = 1;
      overflow0 = OVERFLOW_200HZ;
   }
   else if (level == LEVEL_300) {
      sampleDivider = 2;
      overflow0 = OVERFLOW_300HZ;
   }
   else if (level == LEVEL_400) {
      sampleDivider = 2;
      overflow0 = OVERFLOW_400HZ;
   }
   else{
      sampleDivider = 2;
      overflow0 = OVERFLOW_500HZ;
   }
   OCR0A = overflow0;
}
