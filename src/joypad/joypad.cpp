#include "joypad.h"

Joypad::Joypad(InterruptManager& interruptManager) : interruptManager(interruptManager) {}

u8 Joypad::getJOYP() {
    bool dir = (JOYP & (1 << 4));
    bool but = (JOYP & (1 << 5));

    u8 newState = 0;
    u8 prevState = JOYP;

    if (dir || but) {
        if (!dir) {
            newState = 0xF;
            if (keysPressed[Keys::Right]) newState &= ~1;
            if (keysPressed[Keys::Left]) newState &= ~(1 << 1);
            if (keysPressed[Keys::Up]) newState &= ~(1 << 2);
            if (keysPressed[Keys::Down]) newState &= ~(1 << 3);
        } else if (!but) {
            newState = 0xF;
            if (keysPressed[Keys::A]) newState &= ~1;
            if (keysPressed[Keys::B]) newState &= ~(1 << 1);
            if (keysPressed[Keys::Select]) newState &= ~(1 << 2);
            if (keysPressed[Keys::Start]) newState &= ~(1 << 3);
        }

        if ((newState & 0xF) & ~(prevState & 0xF)) interruptManager.requestInterrupt(InterruptManager::Types::Joypad);
        u8 tmp = (u8)(newState | (prevState & 0x30));
        if ((tmp & 0x30) == 0x10 || (tmp & 0x30) == 0x20) {
            JOYP = tmp;
        } else {
            JOYP = 0xFF;
        }
    }
    return JOYP;
}

void Joypad::setJOYP(u8 value) { JOYP = value; }