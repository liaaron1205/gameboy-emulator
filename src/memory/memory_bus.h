#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "../constants.h"

class MemoryBus {
    std::vector<u8> memory;
    std::vector<u8> cartridge;
    std::vector<u8> ramBanks;  //On the actual hardware, this is external RAM in the cartridge

    u8 currentROMBank = 1;
    u8 currentRAMBank = 0;  //Number from 0-3
    u8 numberRAMBanks;
    bool enableRam = 0;
    bool romBanking = 1;

    int MBC = 0;

   public:
    MemoryBus() = default;
    MemoryBus(std::string filename);
    ~MemoryBus() = default;

    void write(u16 address, u8 data);
    u8 read(u16 address) const;

   private:
    void initialize();

    void HandleBanking(u16 address, u8 data);
    void HandleRAMBankEnable(u16 address, u8 data);
};