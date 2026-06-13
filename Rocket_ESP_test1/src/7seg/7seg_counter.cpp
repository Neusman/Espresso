#include "7seg/7seg_counter.h"

void seg_counter::setupTube() {
    //Wire.begin();
    SegDisp.setTubeType(TYPE_2, TYPE_2_DEFAULT_I2C_ADDR);
    SegDisp.setBrightness(4);
    SegDisp.setBlinkRate(BLINK_OFF);
    SegDisp.clear();
}

void seg_counter::displayNumber(unsigned long currentTime) {
    if (currentTime - _lastCheckTime >= _checkInterval) {
        _lastCheckTime = currentTime;
        num = num + 1;
        SegDisp.displayNum(num);
    }
    //SegDisp.displayNum(num);
}

 void seg_counter::clear() {
    SegDisp.clear();
    num = 0;
 }