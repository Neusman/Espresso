/*
 * 	Interrupt.cpp
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the Tank Level interrupt definition.
 */


#include "Tank.h"

uint8_t Tank::interruptPin = 0;
volatile bool Tank::waterOK = false;
volatile unsigned long Tank::lastInterruptTime = 0;  // Renamed for clarity

void Tank::isr_water() {
  lastInterruptTime = millis();  // Just record the interrupt time
}

void Tank::setup(uint8_t pin) {
  interruptPin = pin;
  pinMode(interruptPin, INPUT);
  waterOK = (digitalRead(interruptPin) == HIGH);  // Initialize
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr_water, CHANGE);
}

bool Tank::WaterIsOK() {
  // Debounce: Only update state if no interrupts for `debounceDelay`
  if (millis() - lastInterruptTime > debounceDelay) {
    waterOK = (digitalRead(interruptPin) == HIGH);
  }
  return waterOK;
}

bool Tank::WaterIsNOK() {
  return !WaterIsOK();  // Uses debounced state
}