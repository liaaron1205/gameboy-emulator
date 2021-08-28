#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "../constants.h"

class MemoryBus {
    std::vector<u8> memory;
    std::vector<u8> cartridge;
    std::vector<u8> ramBanks;
    u8 currentRAMBank;

   public:
    MemoryBus() = default;
    MemoryBus(std::string filename);
    ~MemoryBus() = default;

    void write(u16 address, u8 data);
    u8 read(u16 address);
};