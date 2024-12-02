#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "GPIO.h"

class RotaryEncoder {
private:
    GPIO_HAL gpio;
    int pinA, pinB, pinSW; // GPIO pins for rotary encoder and button
    int counter;           // Counts encoder rotations
    bool lastStateA;       // Previous state of Channel A
    

public:
    RotaryEncoder(int gpioA, int gpioB, int gpioSW = -1);
    bool initialize();
    int update();
    int getCounter() const;
    void resetCounter();
    bool isButtonPressed(bool &pressed);
    void sendKeyEvent(int keycode);
};

#endif
