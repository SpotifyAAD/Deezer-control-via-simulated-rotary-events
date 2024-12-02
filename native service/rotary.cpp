#include "rotary.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>  // For system()

RotaryEncoder::RotaryEncoder(int gpioA, int gpioB, int gpioSW)
    : pinA(gpioA), pinB(gpioB), pinSW(gpioSW), counter(0), lastStateA(false) {}

bool RotaryEncoder::initialize() {
    // Export and set GPIO directions
    if (!gpio.exportGPIO(pinA) || !gpio.exportGPIO(pinB)) {
        std::cerr << "Failed to export GPIO pins!" << std::endl;
        return false;
    }

    if (!gpio.setGPIODirection(pinA, "in") || !gpio.setGPIODirection(pinB, "in")) {
        std::cerr << "Failed to set GPIO directions!" << std::endl;
        return false;
    }

    // Initialize the button GPIO if pinSW is provided
    if (pinSW != -1) {
        if (!gpio.exportGPIO(pinSW) || !gpio.setGPIODirection(pinSW, "in")) {
            std::cerr << "Failed to initialize button GPIO!" << std::endl;
            return false;
        }

        // // Configure the button pin as active-low
        // if (!gpio.setGPIOActiveLow(pinSW, true)) {
        //     std::cerr << "Failed to set GPIO active-low for button!" << std::endl;
        //     return false;
        // }
    }

    // Read initial state of Channel A
    if (!gpio.getGPIOValue(pinA, &lastStateA)) {
        std::cerr << "Failed to read initial state of GPIO A!" << std::endl;
        return false;
    }

    return true;
}

int RotaryEncoder::update() {
    bool stateA, stateB;
    if (gpio.getGPIOValue(pinA, &stateA) && gpio.getGPIOValue(pinB, &stateB)) {
        if (stateA != lastStateA) {
            if (stateB != stateA) {
                counter++; // Clockwise
                std::cout << "Clockwise: Counter = " << counter << std::endl;
                sendKeyEvent(22);  // Send "Next Track" key event
            } else {
                counter--; // Counterclockwise
                std::cout << "Counterclockwise: Counter = " << counter << std::endl;
                sendKeyEvent(21);  // Send "Previous Track" key event
            }
            lastStateA = stateA;
        }
    }

    return counter;
}

int RotaryEncoder::getCounter() const {
    return counter;
}

void RotaryEncoder::resetCounter() {
    counter = 0;
}

bool RotaryEncoder::isButtonPressed(bool &pressed) {
    if (pinSW != -1) {
        if (gpio.getGPIOValue(pinSW, &pressed)) {
            std::cout << "Button state: " << pressed << std::endl;
            if (pressed == false) {  // Button press detected (active-low)
                sendKeyEvent(66);  // Send "Play/Pause" key event
            }
            return true;
        }
        std::cerr << "Failed to read button GPIO value!" << std::endl;
        return false;
    }
    std::cerr << "Button pin not initialized!" << std::endl;
    return false;
}

void RotaryEncoder::sendKeyEvent(int keycode) {
    std::string command = "input keyevent " + std::to_string(keycode);
    system(command.c_str());  // Execute the key event command
    std::cout << "Sent key event: " << keycode << std::endl;
}
