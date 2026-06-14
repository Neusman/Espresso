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
    static volatile bool risingTriggered;
    static volatile bool fallingTriggered;
    static volatile unsigned long lastDebounceTime;
    static const unsigned long debounceDelay = 50; // Debounce delay in ms

    static void isr_lever();

  public:
    static void setup(uint8_t pin,uint8_t mode = HIGH);
    static bool checkTriggeredRising();
    static bool checkTriggeredFalling();
};


#endif /* LEVER_INTERRUPT_H_ */
