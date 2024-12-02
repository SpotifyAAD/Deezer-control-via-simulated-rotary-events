#include "GPIO.h"
#include <fstream>
#include <string>
#include <iostream>


bool GPIO_HAL::exportGPIO(int pin)
{
	std::ofstream exportFile("/sys/class/gpio/export");
	if(!exportFile)
	{
		printf("Faild To Open The file [Export File]");
		return false;
	}

	exportFile <<pin;
	return exportFile.good();

}

bool GPIO_HAL::setGPIODirection(int pin, const std::string &pinDirection)
{
	std::string pinDirectionPath = "/sys/class/gpio/gpio" + std::to_string(pin) + "/direction";
	std::ofstream directionFile(pinDirectionPath);
	if(!directionFile)
		return false;
	directionFile<<pinDirection;
	return directionFile.good();

}


bool GPIO_HAL::setGPIOValue(int pin, bool pinValue)
{
	 std::string pinValuePath="/sys/class/gpio/gpio"+std::to_string(pin)+"/value";
	std::ofstream valueFile(pinValuePath);
        if(!valueFile)
                return false;
        valueFile <<(pinValue?"1":"0");
        return valueFile.good();
}

bool GPIO_HAL::getGPIOValue(int pin, bool *pinValue)
{
    std::string pinValuePath = "/sys/class/gpio/gpio" + std::to_string(pin) + "/value";
    std::ifstream valueFile(pinValuePath);
    if (!valueFile) {
        std::cerr << "Failed to open GPIO value file for pin " << pin << std::endl;
        return false;  // Error logging can help debug this issue
    }

    int gpioValue;
    valueFile >> gpioValue;

    // Validate the value, ensuring it can only be 0 or 1
    if (gpioValue != 0 && gpioValue != 1) {
        std::cerr << "Invalid GPIO value read from pin " << pin << ": " << gpioValue << std::endl;
        return false;
    }

    *pinValue = (gpioValue == 1);
    return valueFile.good();
}


// bool GPIO_HAL::setGPIOActiveLow(int pin, bool activeLow)
// {
//     std::string activeLowPath = "/sys/class/gpio/gpio" + std::to_string(pin) + "/active_low";
//     std::ofstream activeLowFile(activeLowPath);
//     if (!activeLowFile)
//     {
//         printf("Failed To Open The File [Active Low File]");
//         return false;
//     }

//     activeLowFile << (activeLow ? "1" : "0");
//     return activeLowFile.good();
// }
