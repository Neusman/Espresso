/*
 * 	Interrupt.cpp
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the Tank Level interrupt definition.
 */


#include "Tank.h"

uint8_t Tank::interruptPin = 0;
bool Tank::currentState = HIGH;
bool Tank::lastState = HIGH;
unsigned long Tank::lastDebounceTime = 0;

void Tank::setup(uint8_t pin) {
  interruptPin = pin;
  pinMode(interruptPin, INPUT_PULLUP);
  // Initialize state - HIGH means water is OK (float switch open = water present)
  currentState = digitalRead(interruptPin);
  lastState = currentState;
  lastDebounceTime = 0;
}

bool Tank::WaterIsOK() {
  bool reading = digitalRead(interruptPin);
  
  // Debounce check
  if (reading != lastState) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (reading != currentState) {
      currentState = reading;
    }
  }
  lastState = reading;
  
  // Water is OK when sensor reads HIGH (float switch open = water present)
  return currentState == HIGH;
}

bool Tank::WaterIsNOK() {
  return !WaterIsOK();
}