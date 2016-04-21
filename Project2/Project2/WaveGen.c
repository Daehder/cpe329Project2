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

uint16_t volts_to_bits(double voltage){
   double bits = ((voltage/5.0)*4096);
   return bits;
}

void make_square_LUT(){
   int i;
   int duty = NUM_SAMPLES * SQUARE_DUTY/100;
   
   for (i = 0; i < duty; i++)
      SquareWave[i] = volts_to_bits(MAX_VOLTAGE);
   
   for ( ; i < NUM_SAMPLES ; i++)
      SquareWave[i] = volts_to_bits(MIN_VOLTAGE);
}

void make_triangle_LUT(){
   int i;
   int peak = NUM_SAMPLES / 2;
   double increment = ((double) MAX_VOLTAGE - MIN_VOLTAGE) / peak;
   double voltage = -increment;
   
   for (i = 0; i < peak; i++)
      TriWave[i] = volts_to_bits(voltage += increment);
   
   for ( ; i < NUM_SAMPLES; i++)
      TriWave[i] = volts_to_bits(voltage -= increment);
}

void make_sawtooth_LUT(){
   int i;
   
   for (i = 0; i < NUM_SAMPLES; i++){
      
   }
}

void make_sin_LUT(){
   int i;
   
   for (i = 0; i < NUM_SAMPLES; i++){
      
   }
}