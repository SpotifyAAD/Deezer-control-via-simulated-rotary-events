package com.luxoft.hello;

// set gpio
// get gpio
// get value

//@VintfStability
interface IHello{

    int getNumber();

    boolean setGpioValue(int pin, boolean value);

    boolean getGpioValue(int pin);

    int get_reading();

}