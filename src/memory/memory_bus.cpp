#include "memory_bus.h"

MemoryBus::MemoryBus() {
    std::memset(ramBanks, 0, sizeof(ramBanks));
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

void MemoryBus::inc(u16 address) {
    u8 value = read(address);
    write(address, value + 1);
}

void MemoryBus::dec(u16 address) {
    u8 value = read(address);
    write(address, value - 1);
}