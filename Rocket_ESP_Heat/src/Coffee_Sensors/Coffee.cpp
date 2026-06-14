/*
 * 	Interrupt.cpp
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the Tank Level interrupt definition.
 */


#include "Coffee.h"

Coffee::Coffee(int pumpPin, int valvePin, int ledPin, int internalLEDPin)
  : _pumpPin(pumpPin), _valvePin(valvePin), _ledPin(ledPin), _internalLEDPin(internalLEDPin) {
  pinMode(_pumpPin, OUTPUT);
  pinMode(_valvePin, OUTPUT);
  pinMode(_ledPin, OUTPUT);
  pinMode(_internalLEDPin, OUTPUT);
  digitalWrite(_ledPin, OFF);
  digitalWrite(_internalLEDPin, LOW);
}

void Coffee::MakeCoffee() {
  digitalWrite(_pumpPin, ON); 
  digitalWrite(_valvePin, OFF);
}

void Coffee::FillBoiler() {
  digitalWrite(_pumpPin, ON);  
  digitalWrite(_valvePin, ON);
}

void Coffee::NormalState() {
  digitalWrite(_pumpPin, OFF); 
  digitalWrite(_valvePin, OFF); 
}

void Coffee::WaterLevel_LED(bool shouldBlink) {
    static unsigned long lastBlinkTime = 0;
    static bool ledOn = false;

    if (!shouldBlink) {
        // Turn off LED immediately if blinking is disabled
        digitalWrite(_ledPin, OFF);
        digitalWrite(_internalLEDPin, LOW);
        ledOn = false;
        return;
    }

    // Blink at 1Hz (toggle every 500ms)
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= 500) {
        lastBlinkTime = currentMillis;
        ledOn = !ledOn;
        digitalWrite(_ledPin, !ledOn);
        digitalWrite(_internalLEDPin, ledOn);
    }
}