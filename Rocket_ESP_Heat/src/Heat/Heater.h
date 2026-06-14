// Heater.h
#ifndef HEATER_H
#define HEATER_H

#include <Arduino.h>          // For pinMode, digitalWrite, millis, micros, etc.
#include <esp_log.h>         // For ESP_LOGV, ESP_LOGI, ESP_LOGE, ESP_LOGW
#include "PID/Autotune/Autotune.h"
#include "PID/SimplePID/SimplePID.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Define TemperatureSensor interface directly in Heater.h
class TemperatureSensor {
public:
    virtual ~TemperatureSensor() = default;
    virtual float read() = 0;
    virtual bool isErrorState() = 0;
};

enum class PIDLibrary { Legacy, Nimrod };

constexpr float MAX_AUTOTUNE_TEMP = 125.0f;
constexpr float TUNER_OUTPUT_SPAN = 1000.0f;

using heater_error_callback_t = std::function<void()>;
using heater_autotune_fail_callback_t = std::function<void()>;
using pid_result_callback_t = std::function<void(float Kp, float Ki, float Kd, float Kff)>;

class Heater {
public:
    Heater(TemperatureSensor *sensor, uint8_t heaterPin, const heater_error_callback_t &error_callback,
           const pid_result_callback_t &pid_callback, const heater_autotune_fail_callback_t &autotune_fail_callback = nullptr);
    void setup();
    void loop();

    void setSetpoint(float setpoint);
    float getSetpoint() { return setpoint; };
    void setTunings(float Kp, float Ki, float Kd);
    void autotune(int testTimeSec, int windowSize, int heaterWattage);
    
    // Autotune trigger with debounce
    void setAutotuneSwitchPin(uint8_t pin);
    bool checkAutotuneTrigger(int testTimeSec, int windowSize, int heaterWattage);

    // Thermal feedforward control
    void setThermalFeedforward(float *pumpFlowPtr = nullptr, float incomingWaterTemp = 23.0f, int *valveStatusPtr = nullptr);
    void setFeedforwardScale(float combinedKff);

private:
    void setupPid();
    void setupAutotune(int testTimeSec, int windowSize, int heaterWattage);
    void loopPid();
    void loopAutotune();
    float softPwm(uint32_t windowSize);
    void plot(float optimumOutput, float outputScale, uint8_t everyNth);
    float calculateDisturbanceFeedforwardGain();
    float calculateSafetyScaling(float tempError);
    TemperatureSensor *sensor;
    uint8_t heaterPin;
    xTaskHandle taskHandle;
    SimplePID *simplePid = nullptr;
    Autotune *autotuner = nullptr;

    heater_error_callback_t error_callback;
    pid_result_callback_t pid_callback;
    heater_autotune_fail_callback_t autotune_fail_callback;

    float temperature = 0.0f;
    float output = 0.0f;
    float setpoint = 0.0f;
    float Kp = 2.4;
    float Ki = 40;
    float Kd = 10;
    int plotCount = 0;

    bool relayStatus = false;
    unsigned long windowStartTime = 0;
    unsigned long nextSwitchTime = 0;

    // Autotune variables
    bool startup = true;
    bool autotuning = false;
    int autotuneHeaterWattage = 0;
    
    // Autotune trigger debounce variables
    uint8_t autotuneSwitchPin = 255; // Invalid pin by default
    unsigned long lastAutotuneDebounceTime = 0;
    bool lastAutotuneTriggered = false;
    static constexpr unsigned long autotuneDebounceDelay = 1000; // 1 second debounce

    // Thermal feedforward variables
    float *pumpFlowRate = nullptr;
    int *valveStatus = nullptr;
    float lastSafetyFactor = 1.0f;
    float incomingWaterTemp = 23.0f;
    float heaterEfficiency = 0.95f;
    float heatLossWatts = 5.0f;
    float combinedKff = 0.0f;

    // Thermal model constants
    static constexpr float WATER_DENSITY = 1.0f;
    static constexpr float WATER_SPECIFIC_HEAT = 4.18f;

    const char *LOG_TAG = "Heater";
    static void loopTask(void *arg);
};

#endif // HEATER_H