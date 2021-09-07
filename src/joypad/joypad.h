#pragma once

#include "../constants.h"
#include "../cpu/interrupt_manager.h"

class Joypad {
   public:
    enum Keys {
        Right,
        Left,
        Up,
        Down,
        A,
        B,
        Select,
        Start,
    };
    bool keysPressed[8];

   private:
    u8 JOYP;
    InterruptManager& interruptManager;

   public:
    Joypad(InterruptManager& InterruptManager);
    ~Joypad() = default;

    u8 getJOYP();
    void setJOYP(u8 value);
};