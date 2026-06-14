/*
 * 	Interrupt.cpp
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the Tank Level interrupt definition.
 */

#include "Boiler.h"

uint8_t Boiler::interruptPin = 0;
bool Boiler::currentState = LOW;
bool Boiler::lastState = LOW;
unsigned long Boiler::lastDebounceTime = 0;

void Boiler::setup(uint8_t pin) {
  interruptPin = pin;
  pinMode(interruptPin, INPUT_PULLUP);
  // Initialize state - LOW means water is OK (sensor connected to GND when water present)
  currentState = digitalRead(interruptPin);
  lastState = currentState;
  lastDebounceTime = 0;
}

bool Boiler::WaterIsOK() {
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
  
  // Water is OK when sensor reads LOW (connected to GND through water)
  return currentState == LOW;
}

bool Boiler::WaterIsNOK() {
  return !WaterIsOK();
}