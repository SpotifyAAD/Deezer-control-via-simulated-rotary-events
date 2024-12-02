#include "HelloImpl.h"
// #include "gpiohal.h"
// #include "I2C.h"
#include "rotary.h"
#include <android-base/logging.h>


namespace aidl {
namespace com {
namespace luxoft {
namespace hello {

ndk::ScopedAStatus HelloImpl::getUpdate(int32_t* _aidl_return) {
    if (!_aidl_return) {
        return ndk::ScopedAStatus::fromStatus(STATUS_BAD_VALUE);
    }

    LOG(INFO) << "getUpdate called!";

    // Persistent rotary encoder instance
    static RotaryEncoder rotaryEncoder(/*gpioA=*/17, /*gpioB=*/18, /*gpioSW=*/27);

    static bool isInitialized = false;

    // Initialize the rotary encoder only once
    if (!isInitialized) {
        if (!rotaryEncoder.initialize()) {
            LOG(ERROR) << "Failed to initialize the rotary encoder.";
            return ndk::ScopedAStatus::fromStatus(STATUS_UNKNOWN_ERROR);
        }
        isInitialized = true;
        LOG(INFO) << "Rotary encoder initialized.";
    }

    int counterValue;
    int caching = 0;

    

    // Update the encoder state
    do {
        counterValue = rotaryEncoder.update();
        caching = counterValue;
    }
    while(counterValue != caching);

    // Get the updated counter value
    //int counterValue = rotaryEncoder.getCounter();
    *_aidl_return = caching;

    LOG(INFO) << "Rotary encoder counter updated value: " << counterValue;
    
    return ndk::ScopedAStatus::ok();
}


ndk::ScopedAStatus HelloImpl::get_button_state(bool* _aidl_return) {
    if (!_aidl_return) {
        return ndk::ScopedAStatus::fromStatus(STATUS_BAD_VALUE);
    }

    LOG(INFO) << "get_button_state called!";

    static RotaryEncoder rotaryEncoder1(/*gpioA=*/17, /*gpioB=*/18, /*gpioSW=*/27);


    // Check if the button is pressed
    bool isPressed = true;

    

    if (!rotaryEncoder1.isButtonPressed(isPressed)) {
        LOG(ERROR) << "Failed to read the button state!";
        return ndk::ScopedAStatus::fromStatus(STATUS_UNKNOWN_ERROR);
    }

    *_aidl_return = isPressed;

    LOG(INFO) << "Button state: " << (isPressed ? "Pressed" : "Not Pressed");
    return ndk::ScopedAStatus::ok();
}





}  // namespace hello
}  // namespace hardware
}  // namespace aospinsight
}  // namespace aidl
