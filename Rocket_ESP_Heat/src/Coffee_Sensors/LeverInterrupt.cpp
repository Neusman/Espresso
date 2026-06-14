/*
 * 	Interrupt.cpp
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the LEVER interrupt definition.
 */

#include "LeverInterrupt.h"

uint8_t LeverInterrupt::interruptPin = 0; // Default, will be set in setup()
volatile bool LeverInterrupt::risingTriggered = false;
volatile bool LeverInterrupt::fallingTriggered = false;
volatile unsigned long LeverInterrupt::lastDebounceTime;

void LeverInterrupt::isr_lever() {
   unsigned long currentTime = millis();

    // Debounce: ignore if the interrupt was triggered too soon after the last one
    if (currentTime - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentTime;

      // Determine if the interrupt was triggered by a rising or falling edge
      if (digitalRead(interruptPin) == HIGH) {
        risingTriggered = true;
        fallingTriggered = false;  // Clear the other flag
      } else {
        fallingTriggered = true;
        risingTriggered = false;  // Clear the other flag
      }
  }
}

void LeverInterrupt::setup(uint8_t pin,uint8_t mode) {
  interruptPin = pin;
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr_lever, mode);
  lastDebounceTime = 0;
}

bool LeverInterrupt::checkTriggeredRising() {
  bool triggered = risingTriggered;
  risingTriggered = false;
  return triggered;
}

bool LeverInterrupt::checkTriggeredFalling() {
  bool triggered = fallingTriggered;
  fallingTriggered = false;
  return triggered;
}
