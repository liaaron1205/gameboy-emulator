#include "memory_bus.h"

#include <cstdio>

MemoryBus::MemoryBus(std::string filename) : memory(0x10000), cartridge(0x200000), ramBanks(0x8000) {
    std::fill(memory.begin(), memory.end(), 0);
    std::fill(cartridge.begin(), cartridge.end(), 0);
    std::fill(ramBanks.begin(), ramBanks.end(), 0);

    std::ifstream infile(filename, std::ios::binary);

    int idx = 0;
    if (infile.is_open()) {
        while (infile) {
            u8 val = infile.get();
            if (idx < 0x20000) {
                cartridge[idx++] = val;
            }
        }
    }

    memory[0xFF05] = 0x00;
    memory[0xFF06] = 0x00;
    memory[0xFF07] = 0x00;
    memory[0xFF10] = 0x80;
    memory[0xFF11] = 0xBF;
    memory[0xFF12] = 0xF3;
    memory[0xFF14] = 0xBF;
    memory[0xFF16] = 0x3F;
    memory[0xFF17] = 0x00;
    memory[0xFF19] = 0xBF;
    memory[0xFF1A] = 0x7F;
    memory[0xFF1B] = 0xFF;
    memory[0xFF1C] = 0x9F;
    memory[0xFF1E] = 0xBF;
    memory[0xFF20] = 0xFF;
    memory[0xFF21] = 0x00;
    memory[0xFF22] = 0x00;
    memory[0xFF23] = 0xBF;
    memory[0xFF24] = 0x77;
    memory[0xFF25] = 0xF3;
    memory[0xFF26] = 0xF1;
    memory[0xFF40] = 0x91;
    memory[0xFF42] = 0x00;
    memory[0xFF43] = 0x00;
    memory[0xFF45] = 0x00;
    memory[0xFF47] = 0xFC;
    memory[0xFF48] = 0xFF;
    memory[0xFF49] = 0xFF;
    memory[0xFF4A] = 0x00;
    memory[0xFF4B] = 0x00;
    memory[0xFFFF] = 0x00;
}

void MemoryBus::write(u16 address, u8 data) {
    //Read Only
    if (address < 0x8000) {
    }

    //ECHO Ram
    else if (address >= 0xE000 && address < 0xFE00) {
        memory[address] = data;
        write(address - 0x2000, data);
    }

    //Restricted area
    else if (address >= 0xFEA0 && address < 0xFEFF) {
    }

    else if (address == 0xFF44) {
        memory[address] = 0;
    }

    //Handle DMA
    else if (address == 0xFF46) {
    }

    else {
        memory[address] = data;
    }
}

u8 MemoryBus::read(u16 address) {
    //TODO More complex read
    return memory[address];
}