/*
 * 	main.h
 *
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Main file of Rocket Espresso Machine controller V1
 */

#ifndef _main_H_
#define _main_H_
#include "Arduino.h"

#include "Display_OLED/Display.h"
#include "Coffee_Sensors/LeverInterrupt.h"
#include "Coffee_Sensors/Tank.h"
#include "Coffee_Sensors/Boiler.h"
#include "Coffee_Sensors/Coffee.h"
#include "KMeterISO/TempSens.h"
#include "7seg/7seg_counter.h"
#include "Heat/Heater.h"  // Added for Heater class
#include <Wire.h>
#include <M5UnitKmeterISO.h>
#include <Preferences.h>  // Added for NVS


// delay time during startup after each step
int delay_init = 1000; //Time in ms


// Global enum for state machine
enum Coffee_state { NORMAL, MAKING_COFFEE, FILLING_BOILER };
extern Coffee_state currentState;

// 7-segment display
extern seg_counter seg7;

// Heater callbacks
void onHeaterError();
void onPidUpdate(float Kp, float Ki, float Kd, float Kff);
void onAutotuneFail();

// Deep Sleep
extern bool _sleep;
extern unsigned long timedelay;
void IRAM_ATTR isr_stop();

#endif /* _main_H_ */