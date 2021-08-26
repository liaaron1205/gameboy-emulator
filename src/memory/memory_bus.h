#pragma once

#include "../constants.h"

class MemoryBus {
    u8 memory[0x10000];

    u8 ramBanks[0x8000];
    u8 currentRAMBank;

   public:
    void write(u16 address, u8 data);
    u8 read(u16 address);
    void inc(u16 address);
    void dec(u16 address);
};