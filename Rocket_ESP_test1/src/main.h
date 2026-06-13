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
//#include <PID_v2.h>
#include <Wire.h>
#include <M5UnitKmeterISO.h>




//We are already including them in Display class
//#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
//#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"

void IRAM_ATTR isr_stop();
enum Coffee_state { NORMAL, MAKING_COFFEE, FILLING_BOILER };
extern Coffee_state currentState;

#endif /* _main_H_ */
