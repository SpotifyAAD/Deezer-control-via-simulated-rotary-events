# README: Rotary Encoder Service Implementation

## Overview

This project implements a separate service to manage a rotary encoder and button input using GPIO pins. The service resolves a previous issue with button detection by running as a standalone service in the `system` partition rather than `vendor`. It initializes the rotary encoder, polls for button presses, and prints a message when the button is pressed.

---

## Features

1. **Rotary Encoder Initialization and Monitoring:**
   - Uses GPIO pins for rotary encoder inputs (Channel A, Channel B, and button).
   - Continuously monitors for button presses.

2. **Standalone System Service:**
   - Runs as a service in the `system` partition.
   - Automatically starts on system boot using `init.rc` configuration.

3. **Debouncing Mechanism:**
   - Implements basic debouncing with a `200ms` delay when the button is pressed.

4. **Polling Mechanism:**
   - Polls the encoder state every `10ms`.

---

## Project Structure

### 1. **Main Program: `main.cpp`**

The main program initializes the rotary encoder and continuously checks the button state.

#### Key Components:

1. **GPIO Pin Configuration:**
   - `pinA = 17`: GPIO pin for Channel A.
   - `pinB = 18`: GPIO pin for Channel B.
   - `pinSW = 27`: GPIO pin for the button.

2. **Rotary Encoder Initialization:**
   - Calls `encoder.initialize()` to set up the encoder.
   - Displays an error message if initialization fails.

3. **Button Press Detection:**
   - Checks if the button is pressed using `encoder.isButtonPressed(buttonPressed)`.
   - Prints "Button Pressed!" when the button is detected.
   - Includes a `200ms` delay for debouncing.

4. **Polling Delay:**
   - Adds a `10ms` delay between each polling cycle.

#### Code Example:

```cpp
#include "rotary.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    int pinA = 17;
    int pinB = 18;
    int pinSW = 27;

    RotaryEncoder encoder(pinA, pinB, pinSW);

    if (!encoder.initialize()) {
        std::cerr << "Failed to initialize rotary encoder!" << std::endl;
        return 1;
    }

    std::cout << "Rotary encoder initialized. Rotate or press the button to test." << std::endl;

    while (true) {
        bool buttonPressed;
        if (encoder.isButtonPressed(buttonPressed) && buttonPressed) {
            std::cout << "Button Pressed!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
```

---

### 2. **System Service Configuration (`init.rc` script)**

Defines the service that starts the rotary encoder monitoring program during system boot.

#### Key Configuration:

```plaintext
service rotary /system/bin/rotary
    class late_start
    user root
    group root
    seclabel u:object_r:rotary_exec:s0

on boot
    start rotary
```

#### Explanation:

1. **Service Definition:**
   - `service rotary /system/bin/rotary`: Defines the service and specifies the executable path.
   - `class late_start`: Starts the service late in the boot process.
   - `user root`: Runs the service as the `root` user.
   - `group root`: Assigns the `root` group.
   - `seclabel u:object_r:rotary_exec:s0`: Sets the security context.

2. **Auto-Start on Boot:**
   - `on boot`: Ensures the service starts automatically when the system boots.
   - `start rotary`: Starts the `rotary` service.

---



## Troubleshooting

1. **Button Not Detected in Vendor Partition:**
   - The service was moved to the `system` partition because it failed to function in the `vendor` partition. This ensures access to required resources and permissions.

2. **Failed Initialization:**
   - Ensure GPIO pins are correctly configured and accessible.
   - Verify the `gpio` and `rotary` libraries are correctly implemented.

3. **Permission Issues:**
   - Ensure the binary has execute permissions (`chmod 755`).
   - Verify SELinux context is correctly set (`seclabel u:object_r:rotary_exec:s0`).

---

## Notes

- **Polling and Debouncing:** Adjust polling intervals and debounce delays as needed for optimal performance.
- **GPIO Access:** Ensure the device has access to the specified GPIO pins.

