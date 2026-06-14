/*
 * 	main.cpp
 *
 *	Version: 1.0.0
 *  Created on: May 23, 2026
 *  Author: Luc Fiers
 *  Description: Main file of Rocket Espresso Machine controller V1
 */

#include "main.h"

// Temperature Sensor Wrapper (adapts TempSens to TemperatureSensor interface)
class TempSensWrapper : public TemperatureSensor {
public:
    TempSensWrapper(TempSens* sensor) : _sensor(sensor) {}
    float read() override { return _sensor->readTemperatureC(); }
    bool isErrorState() override { return false; }
private:
    TempSens* _sensor;
};

// Global instances
TempSens tempSens(&Wire, 21, 22);
TempSensWrapper tempSensor(&tempSens);
Preferences preferences;

// Heater Configuration
#define HEATER_PIN 15
#define AUTOTUNE_SWITCH_PIN 27
#define HEATER_WATTAGE 1350
#define DEFAULT_SETPOINT 125.0

Heater heater(&tempSensor, HEATER_PIN, onHeaterError, onPidUpdate, onAutotuneFail);

// 7-segment display
seg_counter seg7;  // I2C address: 0x70



// Coffee Instance
Coffee coffee(25, 26, 0);  // Pump=25, Valve=26, LED=0
Coffee_state currentState = NORMAL;

// --- Heater Callbacks ---
void onHeaterError() {
    Serial.println("ERROR: Heater fault or overtemp!");
    digitalWrite(LED_BUILTIN, HIGH);
}

void onPidUpdate(float Kp, float Ki, float Kd, float Kff) {
    Serial.printf("New PID Gains: Kp=%.2f, Ki=%.2f, Kd=%.2f, Kff=%.2f\n", Kp, Ki, Kd, Kff);
    preferences.begin("heater", false);
    preferences.putFloat("Kp", Kp);
    preferences.putFloat("Ki", Ki);
    preferences.putFloat("Kd", Kd);
    preferences.putFloat("Kff", Kff);
    preferences.end();
}

void onAutotuneFail() {
    Serial.println("Autotune Failed: Timeout or no inflection detected.");
}



void setup() {
    Serial.begin(115200);

    // Initialize I2C FIRST
    Wire.begin(21, 22);

    // Initialize sensors
    LeverInterrupt::setup(17, CHANGE);
    Tank::setup(36);
    Boiler::setup(39);
    const uint8_t Sleep_pin = 15;
    coffee.NormalState();

    // Initialize 7-segment display (AFTER Wire.begin)
    seg7.setupTube();
    delay(delay_init);
    Serial.println("7-segment display initialized.");

  

    // Initialize TempSens (KMeterISO)
    if (!tempSens.begin()) {
        Serial.println("Failed to initialize KMeterISO. Check connections!");
        while (1);
    }
    delay(delay_init);
    Serial.println("KMeterISO initialized successfully!");
    delay(delay_init);


    // Initialize heater with NVS-loaded gains
    if (!preferences.begin("heater", true)) {
    Serial.println("Formatting NVS for first use...");
    preferences.begin("heater", false);  // false = read-write mode
    // This will create the namespace
    preferences.end();
    // Now try again
    preferences.begin("heater", true);
    }
    float Kp = preferences.getFloat("Kp", 100.0);
    float Ki = preferences.getFloat("Ki", 10.0);
    float Kd = preferences.getFloat("Kd", 1.0);
    float Kff = preferences.getFloat("Kff", 0.5);
    preferences.end();
    
    Serial.printf("Loaded PID gains: Kp=%.2f, Ki=%.2f, Kd=%.2f, Kff=%.2f\n", Kp, Ki, Kd, Kff);
    delay(delay_init);

    // Setup autotune switch pin
    heater.setAutotuneSwitchPin(AUTOTUNE_SWITCH_PIN);

    heater.setup();
    heater.setTunings(Kp, Ki, Kd);
    heater.setFeedforwardScale(Kff);
    heater.setSetpoint(DEFAULT_SETPOINT);



    delay(delay_init);
    Serial.println("Setup complete. Press internal switch (GPIO 0) to start autotune.");
}

void loop() {
    // Check autotune trigger (with internal 1-second debounce)
    if (heater.checkAutotuneTrigger(120, 6, HEATER_WATTAGE)) {
        Serial.println("Starting Autotune...");
        delay(1000);
    }

    // Heater PID control
    heater.loop();


    // YOUR EXISTING STATE MACHINE (Unchanged)
    if (LeverInterrupt::checkTriggeredRising() && Tank::WaterIsOK()) {
        currentState = MAKING_COFFEE;
        coffee.MakeCoffee();
        Serial.println("Making coffee");
    }

    switch (currentState) {
        case NORMAL:
            if (Boiler::WaterIsNOK() && Tank::WaterIsOK()) {
                currentState = FILLING_BOILER;
                coffee.FillBoiler();
                Serial.println("Fill Boiler");
            }
            break;
        case MAKING_COFFEE:
            // Update 7-segment display with timer
            seg7.displayNumber(millis());
            if (LeverInterrupt::checkTriggeredFalling()) {
                currentState = NORMAL;
                coffee.NormalState();
                seg7.clear();  // Clear display when done
                Serial.println("----");
            }
            break;
        case FILLING_BOILER:
            if (Boiler::WaterIsOK()) {
                currentState = NORMAL;
                coffee.NormalState();
                Serial.println("----1");
            }
            if (Tank::WaterIsNOK()) {
                currentState = NORMAL;
                coffee.NormalState();
                Serial.println("Water empty during boiler fill");
            }
            break;
    }

    // Blink LED if tank water is NOK
    coffee.WaterLevel_LED(Tank::WaterIsNOK());

    // Debug: Print temperature every 20 seconds
    static unsigned long lastTempPrint = 0;
    if (millis() - lastTempPrint > 20000) {
        lastTempPrint = millis();
        Serial.printf("Current Temp: %.2f°C\n", tempSens.readTemperatureC());
    }

    delay(10);
}
