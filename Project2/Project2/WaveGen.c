//
//  WaveGen.c
//  cpe329project2
//
//  Created by Erik Miller on 4/20/16.
//
//

#include "WaveGen.h"

uint16_t SquareWave[NUM_SAMPLES];
uint16_t TriWave[NUM_SAMPLES];
uint16_t SawWave[NUM_SAMPLES];
uint16_t SinWave[NUM_SAMPLES];

/*
uint16_t volts_to_bits(double voltage){
   double bits = ((voltage/5.0)*4096);
   return bits;
}
*/

void make_square_LUT(){
   int i;
   int duty = NUM_SAMPLES * (double) SQUARE_DUTY/100;
   
   for (i = 0; i < duty; i++)
      SquareWave[i] = 0xFFF;
   
   for ( ; i < NUM_SAMPLES ; i++)
      SquareWave[i] = 0x000;
}

void make_triangle_LUT(){
   int i;
   int peak = NUM_SAMPLES / 2;
   double increment = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / peak;
   double voltage = MIN_VOLTAGE - increment;
   
   for (i = 0; i < (peak + 1); i++)
      TriWave[i] = volts_to_bits(voltage += increment);
   
   for ( ; i < (NUM_SAMPLES - 1); i++)
      TriWave[i] = volts_to_bits(voltage -= increment);
}

void make_sawtooth_LUT(){
   int ndx;
   double increment = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / (NUM_SAMPLES - 1);
   double voltage = -increment;
   
   for (ndx = 0; ndx < NUM_SAMPLES; ndx++){
      SawWave[ndx] = volts_to_bits(voltage += increment);
   }
}

void make_sin_LUT(){
   int ndx;
   double increment = 2 * M_PI / NUM_SAMPLES;
   double rad = -increment;
   double ampli = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / 2;
   // The amplitude/2 is neccessary to compensate for the default amplitude of 2
   double offset = MIN_VOLTAGE + ampli;
   
   for (ndx = 0; ndx < NUM_SAMPLES; ndx++){
      SinWave[ndx] = volts_to_bits(ampli * sin(rad += increment) + offset);
   }
}