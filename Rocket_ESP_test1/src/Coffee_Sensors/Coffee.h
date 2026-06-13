/*
 * 	Interrupt.h
 *
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Class providing the Espresso Machine outputs definition.
 */

#ifndef COFFEE_H
#define COFFEE_H

#include <Arduino.h>

class Coffee {
public:
    Coffee(int pumpPin, int valvePin, int ledPin, int internalLEDPin = 2); // Default internal LED pin (GPIO 2)
    void MakeCoffee();
    void FillBoiler();
    void NormalState();
    void WaterLevel_LED(bool shouldBlink = false);

private:
    int _pumpPin;
    int _valvePin;
    int _ledPin;
    int _internalLEDPin;
    bool _ledState = LOW;
    unsigned long _previousMillis = 0;
    bool ON = LOW;
    bool OFF = HIGH;
};

#endif