/*
 * 	Interrupt.h
 *
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the LEVER interrupt definition.
 */

#ifndef TANK_H
#define TANK_H

#include <Arduino.h>

class Tank {
  private:
    static uint8_t interruptPin;
    static volatile bool waterOK;
    static volatile bool waterNOK;
    static volatile unsigned long lastInterruptTime;
    static const unsigned long debounceDelay = 50; // Debounce delay in ms

    static void isr_water();

  public:
    static void setup(uint8_t pin);
    static bool WaterIsOK();
    static bool WaterIsNOK();
};

#endif