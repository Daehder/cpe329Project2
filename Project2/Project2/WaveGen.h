/*
 * Students: Mytch Johnson and Erik Miller
 * Class: CPE 329-07 w/ Dr. Oliver
 * Project 2: Create a function generator
 * Date: 4/20/2016
 * Revision: 1
 */

#ifndef WaveGen_h
#define WaveGen_h

#include <stdio.h>
#include <math.h>
#include "arduinoUtil.h"

void initWaves();
void nextWave();
uint16_t nextWavePoint();
void cycleDuty();

#endif /* WaveGen_h */
