/*
 * 	Interrupt.cpp
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the LEVER interrupt definition.
 */

#include "LeverInterrupt.h"

uint8_t LeverInterrupt::interruptPin = 0; // Default, will be set in setup()
bool LeverInterrupt::currentState = HIGH;
bool LeverInterrupt::lastState = HIGH;
unsigned long LeverInterrupt::lastDebounceTime = 0;

void LeverInterrupt::setup(uint8_t pin, uint8_t mode) {
  interruptPin = pin;
  pinMode(interruptPin, mode);
  // Initialize state
  currentState = digitalRead(interruptPin);
  lastState = currentState;
  lastDebounceTime = 0;
}

bool LeverInterrupt::checkTriggeredRising() {
  bool reading = digitalRead(interruptPin);
  
  // Debounce check
  if (reading != lastState) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (reading != currentState) {
      currentState = reading;
      lastState = reading;
      // Rising edge: was LOW, now HIGH
      return (currentState == HIGH);
    }
  }
  lastState = reading;
  return false;
}

bool LeverInterrupt::checkTriggeredFalling() {
  bool reading = digitalRead(interruptPin);
  
  // Debounce check
  if (reading != lastState) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (reading != currentState) {
      currentState = reading;
      lastState = reading;
      // Falling edge: was HIGH, now LOW
      return (currentState == LOW);
    }
  }
  lastState = reading;
  return false;
}

bool LeverInterrupt::isLeverUp() {
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
  
  return currentState == HIGH;
}

bool LeverInterrupt::isLeverDown() {
  return !isLeverUp();
}
