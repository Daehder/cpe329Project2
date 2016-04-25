/*
 * Students: Mytch Johnson and Erik Miller
 * Class: CPE 329-07 w/ Dr. Oliver
 * Project 2: Create a function generator
 * Date: 4/20/2016
 * Revision: 1
 */

#include "WaveGen.h"

#define NUM_WAVES 4
#define STANDARD_DUTY_CYCLE 50 // %

#define MAX_VOLTAGE 5 // Volts
#define MIN_VOLTAGE 0 // Volts

void make_square_LUT();
void make_triangle_LUT();
void make_sawtooth_LUT();
void make_sin_LUT();

uint8_t dutyCycle = STANDARD_DUTY_CYCLE;

uint16_t SquareWave[NUM_SAMPLES];
uint16_t TriWave[NUM_SAMPLES];
uint16_t SawWave[NUM_SAMPLES];
uint16_t SinWave[NUM_SAMPLES];

uint16_t *Wave;

void initWaves() {
   make_square_LUT();
   make_triangle_LUT();
   make_sawtooth_LUT();
   make_sin_LUT();
   
   Wave = SquareWave;
}

void nextWave() {
   static uint8_t waveNdx = 0;
   static uint16_t* waves[NUM_WAVES] = {SquareWave, TriWave, SawWave, SinWave};
   
   waveNdx++;
   waveNdx %= NUM_WAVES;
   
   Wave = waves[waveNdx];
}

uint16_t nextWavePoint() {
   static uint8_t ndx = -1;
   
   ndx += sampleDivider;
   ndx %= NUM_SAMPLES;
   
   return Wave[ndx];
}

void cycleDuty() {
   dutyCycle += 10;
   dutyCycle %= 110;
   make_square_LUT();
}

void make_square_LUT(){
   uint8_t i;
   uint8_t duty = NUM_SAMPLES * (double) dutyCycle/100;
   
   for (i = 0; i < duty; i++)
      SquareWave[i] = volts_to_bits(MAX_VOLTAGE);
   
   for ( ; i < NUM_SAMPLES ; i++)
      SquareWave[i] = volts_to_bits(MIN_VOLTAGE);
}

void make_triangle_LUT(){
   uint8_t i;
   uint8_t peak = NUM_SAMPLES / 2;
   double increment = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / peak;
   double voltage = MIN_VOLTAGE - increment;
   
   for (i = 0; i < (peak + 1); i++)
      TriWave[i] = volts_to_bits(voltage += increment);
   
   for ( ; i < (NUM_SAMPLES - 1); i++)
      TriWave[i] = volts_to_bits(voltage -= increment);
}

void make_sawtooth_LUT(){
   uint8_t ndx;
   double increment = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / (NUM_SAMPLES - 1);
   double voltage = MIN_VOLTAGE - increment;
   
   for (ndx = 0; ndx < NUM_SAMPLES; ndx++)
      SawWave[ndx] = volts_to_bits(voltage += increment);
}

void make_sin_LUT(){
   uint8_t ndx;
   double increment = 2 * M_PI / NUM_SAMPLES;
   double rad = -increment;
   double ampli = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / 2;
   // The amplitude/2 is neccessary to compensate for sin()'s amplitude of 2
   double offset = MIN_VOLTAGE + ampli;
   
   for (ndx = 0; ndx < NUM_SAMPLES; ndx++)
      SinWave[ndx] = volts_to_bits(ampli * sin(rad += increment) + offset);
}