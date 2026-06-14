/*
 * 	Interrupt.h
 *
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the LEVER interrupt definition.
 */

#ifndef LEVER_INTERRUPT_H
#define LEVER_INTERRUPT_H

#include <Arduino.h>

class LeverInterrupt {
  private:
    static uint8_t interruptPin;
    static bool currentState;
    static bool lastState;
    static unsigned long lastDebounceTime;
    static const unsigned long debounceDelay = 50; // Debounce delay in ms
    
    // For edge detection
    static bool lastRisingEdge;
    static bool lastFallingEdge;

  public:
    static void setup(uint8_t pin, uint8_t mode = INPUT_PULLUP);
    static void update(); // Call this to update internal state with debounce
    static bool checkTriggeredRising();
    static bool checkTriggeredFalling();
    static bool isLeverUp();
    static bool isLeverDown();
};


#endif /* LEVER_INTERRUPT_H_ */
