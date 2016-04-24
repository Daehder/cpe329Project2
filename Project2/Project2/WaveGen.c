//
//  WaveGen.c
//  cpe329project2
//
//  Created by Erik Miller on 4/20/16.
//
//

#include "WaveGen.h"

#define NUM_WAVES 4

void make_square_LUT();
void make_triangle_LUT();
void make_sawtooth_LUT();
void make_sin_LUT();

int dutyCycle = 50;  // The Duty cycle of the square wave in percentage,
                     //  must be between 0 and 100
int _sampleNdx = -1;

uint16_t SquareWave[NUM_SAMPS];
uint16_t TriWave[NUM_SAMPS];
uint16_t SawWave[NUM_SAMPS];
uint16_t SinWave[NUM_SAMPS];

uint16_t *Wave;

//uint16_t volts_to_bits(double voltage){
//   double bits = ((voltage/5.0)*4095);
//   if(bits>4095)
//      return 4095;
//   else
//      return bits;                  // return 12bit equivalent for DAC
//}

void initWaves() {
   make_square_LUT();
   make_triangle_LUT();
   make_sawtooth_LUT();
   make_sin_LUT();
   
   Wave = SquareWave;
}

void nextWave() {
   static int waveNdx = 0;
   static uint16_t* waves[NUM_WAVES] = {SquareWave, TriWave, SawWave, SinWave};
   
   waveNdx++;
   waveNdx %= NUM_WAVES;
   
   Wave = waves[waveNdx];
}

uint16_t nextWavePoint() {
   static int ndx = -1;
   
   ndx++;
   ndx %= NUM_SAMPS;
   
   return Wave[ndx];
}

void cycleDuty() {
   dutyCycle += 10;
   dutyCycle %= 110;
   make_square_LUT();
}

void make_square_LUT(){
   int i;
   int duty = num_samples * (double) dutyCycle/100;
   
   for (i = 0; i < duty; i++)
      SquareWave[i] = volts_to_bits(MAX_VOLTAGE);
   
   for ( ; i < num_samples ; i++)
      SquareWave[i] = volts_to_bits(MIN_VOLTAGE);
}

void make_triangle_LUT(){
   int i;
   int peak = num_samples / 2;
   double increment = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / peak;
   double voltage = MIN_VOLTAGE - increment;
   
   for (i = 0; i < (peak + 1); i++)
      TriWave[i] = volts_to_bits(voltage += increment);
   
   for ( ; i < (num_samples - 1); i++)
      TriWave[i] = volts_to_bits(voltage -= increment);
}

void make_sawtooth_LUT(){
   int ndx;
   double increment = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / (num_samples - 1);
   double voltage = MIN_VOLTAGE -increment;
   
   for (ndx = 0; ndx < num_samples; ndx++)
      SawWave[ndx] = volts_to_bits(voltage += increment);
}

void make_sin_LUT(){
   int ndx;
   double increment = 2 * M_PI / num_samples;
   double rad = -increment;
   double ampli = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / 2;
   // The amplitude/2 is neccessary to compensate for sin()'s amplitude of 2
   double offset = MIN_VOLTAGE + ampli;
   
   for (ndx = 0; ndx < num_samples; ndx++)
      SinWave[ndx] = volts_to_bits(ampli * sin(rad += increment) + offset);
}