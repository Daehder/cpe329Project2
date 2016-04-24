//
//  WaveGen.h
//  cpe329project2
//
//  Created by Erik Miller on 4/20/16.
//
//

#ifndef WaveGen_h
#define WaveGen_h

//#define num_samples 50			// sets global number of samples

#define MAX_VOLTAGE 5 // Volts
#define MIN_VOLTAGE 0 // Volts

//#define NUM_SAMPS 50        // For Erik's testing purposes
//#define num_samples 50

#include <stdio.h>
#include <math.h>
//#include "arduinoUtil.h"

void initWaves();
void nextWave();
uint16_t nextWavePoint();
void cycleDuty();

extern uint16_t *Wave;

#endif /* WaveGen_h */
