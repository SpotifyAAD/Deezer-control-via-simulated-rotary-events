# README: Rotary Encoder and GPIO HAL Interface

## Overview

This project demonstrates how to interface with a rotary encoder using GPIO in a Linux environment. It consists of two main components:

1. **`GPIO_HAL`**: A Hardware Abstraction Layer (HAL) for general-purpose I/O (GPIO) operations.
2. **`RotaryEncoder`**: A class to handle rotary encoder input and optional button press events.

## Project Structure

- **`GPIO_HAL` Class (`GPIO.h` and `GPIO.cpp`)**
  - Manages GPIO pin export, direction setting, and value manipulation.
- **`RotaryEncoder` Class (`rotary.h` and `rotary.cpp`)**
  - Tracks rotary encoder rotation and button presses, sending key events when actions are detected.

---

## Files

### 1. `GPIO_HAL` Class (GPIO.h / GPIO.cpp)
Provides low-level control of GPIO pins using the Linux file system.

#### Methods:

- **`bool exportGPIO(int pin)`**
  - Exports the specified GPIO pin for user control.
  - Writes the pin number to `/sys/class/gpio/export`.
  - **Returns:** `true` if successful, `false` otherwise.

- **`bool setGPIODirection(int pin, const std::string &pinDirection)`**
  - Sets the GPIO pin direction (`in` or `out`).
  - Writes to `/sys/class/gpio/gpio<pin>/direction`.
  - **Returns:** `true` if successful, `false` otherwise.

- **`bool setGPIOValue(int pin, bool pinValue)`**
  - Sets the output value (`1` or `0`) for the specified pin.
  - Writes to `/sys/class/gpio/gpio<pin>/value`.
  - **Returns:** `true` if successful, `false` otherwise.

- **`bool getGPIOValue(int pin, bool *pinValue)`**
  - Reads the current value (`1` or `0`) of the specified pin.
  - Reads from `/sys/class/gpio/gpio<pin>/value`.
  - **Returns:** `true` if successful, `false` otherwise.

---

### 2. `RotaryEncoder` Class (rotary.h / rotary.cpp)
Handles rotary encoder input and optional button press detection.

#### Attributes:

- **`GPIO_HAL gpio`**: HAL object for GPIO manipulation.
- **`int pinA, pinB, pinSW`**: GPIO pins for the encoder and button.
- **`int counter`**: Tracks encoder rotations.
- **`bool lastStateA`**: Stores the last state of Channel A.

#### Methods:

- **`RotaryEncoder(int gpioA, int gpioB, int gpioSW = -1)`**
  - Constructor initializing the GPIO pins and counter.

- **`bool initialize()`**
  - Exports GPIO pins and sets them as input.
  - Initializes the button if `pinSW` is provided.
  - Reads the initial state of Channel A.
  - **Returns:** `true` if initialization is successful.

- **`int update()`**
  - Reads the current state of the encoder pins.
  - Updates the counter based on the rotation direction.
  - Sends key events for clockwise (`22`) and counterclockwise (`21`) rotations.
  - **Returns:** The current counter value.

- **`int getCounter() const`**
  - Returns the current counter value.

- **`void resetCounter()`**
  - Resets the counter to `0`.

- **`bool isButtonPressed(bool &pressed)`**
  - Checks if the button is pressed.
  - Sends a key event (`66`) for "Play/Pause" if pressed.
  - **Returns:** `true` if successful.

- **`void sendKeyEvent(int keycode)`**
  - Executes a system command to send a key event using `input keyevent <keycode>`.

---

## Usage Example

### 1. Initialize the Rotary Encoder:

```cpp
int main() {
    RotaryEncoder encoder(17, 18, 27);  // GPIO pins for Channel A, B, and button
    if (!encoder.initialize()) {
        std::cerr << "Failed to initialize Rotary Encoder!" << std::endl;
        return -1;
    }

    while (true) {
        encoder.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
```

### 2. Reading Button Presses:

```cpp
bool pressed;
if (encoder.isButtonPressed(pressed)) {
    if (!pressed) {
        std::cout << "Button Pressed!" << std::endl;
    }
}
```

---

## Key Event Mappings

| Event           | Key Code | Description          |
|-----------------|-----------|----------------------|
| Clockwise       | 22        | "Next Track"         |
| Counterclockwise| 21        | "Previous Track"     |
| Button Press    | 66        | "Play/Pause"         |

---

## Dependencies

- **C++11 or higher**
- Linux system with GPIO access
- GCC or compatible compiler

---

## Compilation and Execution

1. Compile the project:

   ```bash
   g++ -std=c++11 -o rotary_encoder main.cpp GPIO.cpp rotary.cpp
   ```

2. Run the program:

   ```bash
   sudo ./rotary_encoder
   ```

---

## Notes

- Ensure proper permissions are set to access GPIO files (`/sys/class/gpio`).
- `system()` calls are used for key event triggering; replace them if necessary for better security or performance.

---

## License

This project is open-source. Feel free to modify and distribute as needed.

---

## Contact

For any questions or issues, please contact the developer or open an issue on the project's repository.
