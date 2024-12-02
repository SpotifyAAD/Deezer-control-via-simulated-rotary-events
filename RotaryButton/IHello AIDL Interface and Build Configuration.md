# README: `IHello` AIDL Interface and Build Configuration

## Overview

This project defines an AIDL (`Android Interface Definition Language`) interface named `IHello`, which provides methods for interacting with hardware GPIO and retrieving data such as rotary encoder updates. The interface is designed to maintain `VINTF` (Vendor Interface Versioning) stability, ensuring compatibility across Android system updates.

## Features

1. **GPIO Interaction (Commented Out in This Version):**
   - Setting and reading GPIO values (currently commented out for future use).
   
2. **Rotary Encoder Integration:**
   - Retrieves the rotary encoder's update via `getUpdate()`.

3. **Vendor-Specific Interface:**
   - Uses VINTF stability for consistent interaction with hardware across Android versions.

---

## Project Structure

### 1. **`IHello.aidl` (AIDL Interface)**

Defines the interface that the Android system uses to interact with hardware components.

#### Methods in the AIDL:

1. **`int getUpdate()`**
   - Returns the current rotary encoder counter value.
   - This method is the primary active method in this version of the interface.

#### Commented Methods for Future Implementation:

1. **`boolean setGpioValue(int pin, boolean value)`**
   - Sets the value of the specified GPIO pin.
   - Expected to return `true` on success.

2. **`boolean getGpioValue(int pin)`**
   - Reads the value of the specified GPIO pin.
   - Expected to return `true` if the pin is high, `false` otherwise.

3. **`int get_reading()`**
   - Placeholder for retrieving additional data from hardware.

4. **`boolean get_button_state()`**
   - Intended for checking if a button is pressed (currently commented out).

### 2. **`aidl_interface` (Build Configuration)**

Defines the build configuration for the AIDL interface in the Android build system.

#### Key Fields in the Build Configuration:

- **`name`**: `"com.luxoft.hello"`
  - The package name for the AIDL interface.

- **`srcs`**: `["com/luxoft/hello/*.aidl"]`
  - Specifies the location of the AIDL source files.

- **`stability`**: `"vintf"`
  - Ensures the interface is VINTF-stable for consistent behavior across system updates.

- **`vendor_available`**: `true`
  - Allows the interface to be available for vendor implementations.

- **`backend`**: Configuration for the AIDL backend.
  - **`java: { platform_apis: true }`**
    - Enables access to platform APIs using Java.

- **`owner`**: `"Luxoft"`
  - Specifies the interface owner for organizational reference.

- **`versions_with_info`**: Versioning information for the interface.
  - **`version: "1"`**: Indicates the initial version.
  - **`imports: []`**: No external dependencies in this version.

- **`frozen: true`**: Prevents further modification of the current interface version to maintain backward compatibility.

---

## Dependencies

- **AOSP AIDL Framework**
- **Android VINTF Compatibility Layer**

---

## Compilation

### 1. **Building AIDL Interface:**

Ensure that the AIDL files are correctly configured in the `Android.bp` build file. Run the following commands to build the interface:

```bash
make com.luxoft.gpio-freeze-api
make com.luxoft.gpio-update-api
```

### 2. **Verifying VINTF Stability:**

After building, verify that the interface meets the VINTF stability requirements:

```bash
vintf-stability-check
```

---

## Usage

### Registering the Interface:

To use this interface in an Android environment, register it using the `ServiceManager`:

```java
IHello helloService = IHello.Stub.asInterface(
    ServiceManager.getService("com.luxoft.hello"));
```




