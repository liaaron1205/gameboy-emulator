#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "../constants.h"
#include "../cpu/interrupt_manager.h"
#include "../gpu/gpu.h"
#include "../timer/timer.h"

class MemoryBus {
   public:
    std::vector<u8> memory;
    std::vector<u8> cartridge;
    std::vector<u8> ramBanks;  //On the actual hardware, this is external RAM in the cartridge

    u8 currentROMBank = 1;
    u8 currentRAMBank = 0;  //Number from 0-3
    u8 numberRAMBanks;
    bool enableRam = 0;
    bool romBanking = 1;

    int MBC = 0;

    InterruptManager& interruptManager;
    Timer& timer;
    GPU& gpu;

    struct DMA {
        bool transferInProgress;
        bool transferRestarted;
        u16 from;
        int ticks;
        u8 DMARegister = 0xFF;

        bool isOamBlocked();
        void setDMARegister(u8 value);
    } dma;

   public:
    MemoryBus(InterruptManager& interruptManager, Timer& timer, GPU& gpu);
    ~MemoryBus() = default;

    void tick();

    void loadCartridge(std::string filename);

    void write(u16 address, u8 data);
    u8 read(u16 address);

   private:
    void initialize();

    void writeIO(u16 address, u8 data);
    u8 readIO(u16 address);

    void HandleBanking(u16 address, u8 data);
    void HandleRAMBankEnable(u16 address, u8 data);
};