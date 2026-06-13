/*
 * 	Interrupt.cpp
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the Tank Level interrupt definition.
 */

#include "Boiler.h"

uint8_t Boiler::interruptPin = 0;
volatile bool Boiler::waterOK = false;
volatile unsigned long Boiler::lastInterruptTime = 0;  // Renamed for clarity

void Boiler::isr_water() {
  lastInterruptTime = millis();  // Just record the interrupt time
}

void Boiler::setup(uint8_t pin) {
  interruptPin = pin;
  pinMode(interruptPin, INPUT);
  waterOK = (digitalRead(interruptPin) == LOW);  // Initialize
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr_water, CHANGE);
}

bool Boiler::WaterIsOK() {
  // Debounce: Only update state if no interrupts for `debounceDelay`
  if (millis() - lastInterruptTime > debounceDelay) {
    waterOK = (digitalRead(interruptPin) == LOW);
  }
  return waterOK;
}

bool Boiler::WaterIsNOK() {
  return !WaterIsOK();  // Uses debounced state
}