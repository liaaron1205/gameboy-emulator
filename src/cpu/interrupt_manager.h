#pragma once

#include "../constants.h"

class InterruptManager {
   public:  //TODO REmove
    u8 IE = 0xFF, IF = 0xE0;
    bool IME = 0;

    std::vector<u16> addresses = {0x0040, 0x0048, 0x0050, 0x0058, 0x0060};

   public:
    enum Types {
        Vblank,
        LCD,
        Timer,
        SerialLink,
        Joypad,
    };

   public:
    InterruptManager() = default;
    ~InterruptManager() = default;

    void setIE(u8 value);
    void setIF(u8 value);
    void setIME(bool value);

    u8 getIE();
    u8 getIF();

    void requestInterrupt(Types type);

    //Returns the address of the highest priority interrupt.
    u16 checkInterrupts(bool overrideIME = 0);
};