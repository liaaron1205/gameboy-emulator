#include "timer.h"

Timer::Timer(InterruptManager& interruptManager) : interruptManager(interruptManager) {}

void Timer::tick() {
    updateDiv((timerCounter + 1) & 0xFFFF);
    if (!overflow) return;

    cyclesSinceOverflow++;
    if (cyclesSinceOverflow == 4) {
        interruptManager.requestInterrupt(InterruptManager::Timer);
    }

    if (cyclesSinceOverflow == 5) tima = tma;

    if (cyclesSinceOverflow == 6) {
        tima = tma;
        overflow = 0;
        cyclesSinceOverflow = 0;
    }
}

u8 Timer::getDiv() { return (u8)(timerCounter >> 8); }

void Timer::setDiv() { updateDiv(0); }

u8 Timer::getTima() { return tima; }

void Timer::setTima(u8 value) {
    if (cyclesSinceOverflow < 5) {
        tima = value;
        overflow = 0;
        cyclesSinceOverflow = 0;
    }
}

u8 Timer::getTma() { return tma; }

void Timer::setTma(u8 value) { tma = value; }

u8 Timer::getTac() { return (u8)(tac | 0b11111000); }

void Timer::setTac(u8 value) { tac = value; }

void Timer::updateDiv(int newTimerCounter) {
    timerCounter = newTimerCounter;

    int bitPos = freqToBit[tac & 0b11];

    bool bit = (timerCounter & (1 << bitPos));
    bit = bit && (tac & 0b100);
    if (!bit && previousBit) {
        updateTima();
    }
    previousBit = bit;
}

void Timer::updateTima() {
    tima++;
    tima %= 256;
    if (tima == 0) {
        overflow = 1;
        cyclesSinceOverflow = 0;
    }
}