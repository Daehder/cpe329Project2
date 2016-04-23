//
//  WaveGen.h
//  cpe329project2
//
//  Created by Erik Miller on 4/20/16.
//
//

#ifndef WaveGen_h
#define WaveGen_h

//#define NUM_SAMPLES 200			// sets global number of samples

#define MAX_VOLTAGE 5 // Volts
#define MIN_VOLTAGE 0 // Volts

#define SQUARE_DUTY 50	//%

#include <stdio.h>
#include <math.h>
#include "arduinoUtil.h"

uint16_t volts_to_bits(double voltage);
void make_square_LUT();
void make_triangle_LUT();
void make_sawtooth_LUT();
void make_sin_LUT();

extern uint16_t SquareWave[];
extern uint16_t TriWave[];
extern uint16_t SawWave[];
extern uint16_t SinWave[];

#endif /* WaveGen_h */
