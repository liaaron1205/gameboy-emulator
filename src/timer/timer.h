#pragma once

#include "../constants.h"
#include "../cpu/interrupt_manager.h"

class Timer {
    const std::vector<int> freqToBit = {9, 3, 5, 7};

    InterruptManager& interruptManager;

    int timerCounter, cyclesSinceOverflow;
    u8 tima, tac, tma;
    bool overflow = 0, previousBit = 0;

   public:
    Timer(InterruptManager& interruptManager);
    ~Timer() = default;

    u8 getDiv();
    void setDiv();

    u8 getTima();
    void setTima(u8 value);

    u8 getTma();
    void setTma(u8 value);

    u8 getTac();
    void setTac(u8 value);

    void tick();

   private:
    void updateDiv(int newTimerCounter);

    void updateTima();
};