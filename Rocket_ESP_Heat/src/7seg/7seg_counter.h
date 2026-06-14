// 7seg_counter.h
#ifndef SEG7_COUNTER_H
#define SEG7_COUNTER_H

#include <Wire.h>
#include <grove_alphanumeric_display.h>

class seg_counter {
public:
    void setupTube();
    void displayNumber(unsigned long currentTime);
    void clear();

private:
    Seeed_Digital_Tube SegDisp;
    unsigned long _lastCheckTime = 0;
    const unsigned long _checkInterval = 1000; // 1000 ms interval
    uint8_t num = 0;
};

#endif // GROVE_TUBE_H