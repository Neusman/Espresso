// TempSens.h
#ifndef TEMPSENS_H
#define TEMPSENS_H

#include <Arduino.h>
#include <Wire.h>
#include <M5UnitKmeterISO.h>

class TempSens {
public:
    // Constructor
    TempSens(TwoWire* wire, uint8_t sdaPin, uint8_t sclPin, uint32_t i2cSpeed = 100000L);

    // Initialize the KMeterISO sensor
    bool begin(uint8_t address = KMETER_DEFAULT_ADDR);

    // Read temperature in Celsius
    float readTemperatureC();

    // Check if the sensor is ready
    uint8_t getReadyStatus();

    // Continuously check and print temperature at a fixed interval
    void checkAndPrintTemperature(unsigned long currentTime);

private:
    M5UnitKmeterISO _kmeter;
    TwoWire* _wire;
    uint8_t _sdaPin;
    uint8_t _sclPin;
    uint32_t _i2cSpeed;
    uint8_t _address;
    unsigned long _lastCheckTime = 0;
    const unsigned long _checkInterval = 5000; // 5000 ms interval
};

#endif // TEMPSENS_H