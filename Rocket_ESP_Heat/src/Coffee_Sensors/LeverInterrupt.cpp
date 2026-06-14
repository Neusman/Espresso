/*
 * 	Interrupt.cpp
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the LEVER interrupt definition.
 */

#include "LeverInterrupt.h"

uint8_t LeverInterrupt::interruptPin = 0;
bool LeverInterrupt::currentState = HIGH;
bool LeverInterrupt::lastState = HIGH;
unsigned long LeverInterrupt::lastDebounceTime = 0;
bool LeverInterrupt::lastRisingEdge = false;
bool LeverInterrupt::lastFallingEdge = false;

void LeverInterrupt::setup(uint8_t pin, uint8_t mode) {
  interruptPin = pin;
  pinMode(interruptPin, mode);
  currentState = digitalRead(interruptPin);
  lastState = currentState;
  lastDebounceTime = 0;
  lastRisingEdge = false;
  lastFallingEdge = false;
}

void LeverInterrupt::update() {
  bool reading = digitalRead(interruptPin);
  
  // If the input changed due to noise or pressing
  if (reading != lastState) {
    lastDebounceTime = millis();
  }
  
  // Whatever the reading is at, it's been there for longer than the debounce delay
  if (millis() - lastDebounceTime > debounceDelay) {
    // If the button state has changed
    if (reading != currentState) {
      bool wasLow = (currentState == LOW);
      currentState = reading;
      
      // Detect edges
      if (currentState == HIGH && wasLow) {
        lastRisingEdge = true;
      } else if (currentState == LOW && !wasLow) {
        lastFallingEdge = true;
      }
    }
  }
  
  lastState = reading;
}

bool LeverInterrupt::checkTriggeredRising() {
  update();
  if (lastRisingEdge) {
    lastRisingEdge = false;
    return true;
  }
  return false;
}

bool LeverInterrupt::checkTriggeredFalling() {
  update();
  if (lastFallingEdge) {
    lastFallingEdge = false;
    return true;
  }
  return false;
}

bool LeverInterrupt::isLeverUp() {
  update();
  return currentState == HIGH;
}

bool LeverInterrupt::isLeverDown() {
  update();
  return currentState == LOW;
}
