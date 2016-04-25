#ifndef WaveGen_h
#define WaveGen_h

#define MAX_VOLTAGE 5 // Volts
#define MIN_VOLTAGE 0 // Volts

#include <stdio.h>
#include <math.h>
#include "arduinoUtil.h"

void initWaves();
void nextWave();
uint16_t nextWavePoint();
void cycleDuty();

#endif /* WaveGen_h */
