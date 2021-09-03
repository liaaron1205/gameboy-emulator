
#include "interrupt_manager.h"

#include <iostream>

void InterruptManager::setIE(u8 value) { IE = value; }

void InterruptManager::setIF(u8 value) { IF = value | 0xE0; }

u8 InterruptManager::getIE() { return IE; }

u8 InterruptManager::getIF() { return IF; }

void InterruptManager::setIME(bool value) { IME = value; }

void InterruptManager::requestInterrupt(InterruptManager::Types type) {
    setIF(IF | (1 << type));
}

u16 InterruptManager::checkInterrupts(bool overrideIME) {
    if (!IME && !overrideIME) return 0xFFFF;
    for (int type = 0; type < 5; type++) {
        u8 mask = (1 << type);
        if ((IE & mask) && (IF & mask)) {
            if (!overrideIME) {
                IME = 0;
                IF &= ~mask;
            }
            return addresses[type];
        }
    }
    return 0xFFFF;
}