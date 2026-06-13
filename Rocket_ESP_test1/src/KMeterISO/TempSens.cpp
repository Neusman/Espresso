// TempSens.cpp
#include "TempSens.h"

TempSens::TempSens(TwoWire* wire, uint8_t sdaPin, uint8_t sclPin, uint32_t i2cSpeed)
    : _wire(wire), _sdaPin(sdaPin), _sclPin(sclPin), _i2cSpeed(i2cSpeed) {}

bool TempSens::begin(uint8_t address) {
    _address = address;
    // Do NOT call _wire->begin() here (already done in setup())
    return _kmeter.begin(_wire, _address, _sdaPin, _sclPin, _i2cSpeed);
}

float TempSens::readTemperatureC() {
    if (getReadyStatus() != 0) {
        Serial.println("Error: KMeterISO not ready!");
        return -1.0f;
    }
    return ((float)_kmeter.getCelsiusTempValue()) / 100.0f;
}

uint8_t TempSens::getReadyStatus() {
    return _kmeter.getReadyStatus();
}

void TempSens::checkAndPrintTemperature(unsigned long currentTime) {
    if (currentTime - _lastCheckTime >= _checkInterval) {
        _lastCheckTime = currentTime;
        float temperature = readTemperatureC();
        Serial.printf("Temperature: %.2f °C\n", temperature);
    }
}