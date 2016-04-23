//
//  WaveGen.c
//  cpe329project2
//
//  Created by Erik Miller on 4/20/16.
//
//

#include "WaveGen.h"

uint16_t SquareWave[num_samples];
uint16_t TriWave[num_samples];
uint16_t SawWave[num_samples];
uint16_t SinWave[num_samples];

uint16_t Wave[num_samples]

/*
uint16_t volts_to_bits(double voltage){
   double bits = ((voltage/5.0)*4096);
   return bits;
}
*/

void make_square_LUT(){
   int i;
   int duty = num_samples * (double) SQUARE_DUTY/100;
   
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
   double voltage = -increment;
   
   for (ndx = 0; ndx < num_samples; ndx++){
      SawWave[ndx] = volts_to_bits(voltage += increment);
   }
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