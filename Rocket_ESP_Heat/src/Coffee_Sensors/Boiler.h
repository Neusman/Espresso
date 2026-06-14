/*
 * 	Interrupt.h
 *
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the LEVER interrupt definition.
 */

#ifndef BOILER_H
#define BOILER_H

#include <Arduino.h>

class Boiler {
  private:
    static uint8_t interruptPin;
    static bool currentState;
    static bool lastState;
    static unsigned long lastDebounceTime;
    static const unsigned long debounceDelay = 1000; // Debounce delay in ms

  public:
    static void setup(uint8_t pin);
    static bool WaterIsOK();
    static bool WaterIsNOK();
};

#endif
