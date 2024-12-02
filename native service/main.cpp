#include "rotary.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    int pinA = 17; // Replace with your GPIO pin for Channel A
    int pinB = 18; // Replace with your GPIO pin for Channel B
    int pinSW = 27; // Replace with your GPIO pin for button (optional)

    RotaryEncoder encoder(pinA, pinB, pinSW);

    if (!encoder.initialize()) {
        std::cerr << "Failed to initialize rotary encoder!" << std::endl;
        return 1;
    }

    std::cout << "Rotary encoder initialized. Rotate or press the button to test." << std::endl;

    while (true) {
        // encoder.update();
        
        // Optional: Check button state
        bool buttonPressed;
        if (encoder.isButtonPressed(buttonPressed) && buttonPressed) {
            std::cout << "Button Pressed!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Debounce
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Polling delay
    }

    return 0;
}
