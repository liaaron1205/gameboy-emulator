#include "memory_bus.h"

#include <cstdio>

MemoryBus::MemoryBus(InterruptManager& interruptManager, Timer& timer, GPU& gpu) : memory(0x10000), cartridge(0x200000), ramBanks(0x8000), interruptManager(interruptManager), timer(timer), gpu{gpu} {
    std::fill(memory.begin(), memory.end(), 0);
    std::fill(cartridge.begin(), cartridge.end(), 0);
    std::fill(ramBanks.begin(), ramBanks.end(), 0);

    numberRAMBanks = cartridge[0x148];
}

//TODO use write fn instead
void MemoryBus::initialize() {
    write(0xFF00, 0xCF);  // JOYP
    write(0xFF02, 0x7E);  // SC
    write(0xFF04, 0xAB);  // DIV
    write(0xFF08, 0xF8);  // TAC
    write(0xFF0F, 0xE1);  // IF
    write(0xFF10, 0x80);  // ENT1
    write(0xFF11, 0xBF);  // LEN1
    write(0xFF12, 0xF3);  // ENV1
    write(0xFF13, 0xC1);  // FRQ1
    write(0xFF14, 0xBF);  // KIK1
    write(0xFF15, 0xFF);  // N/A
    write(0xFF16, 0x3F);  // LEN2
    write(0xFF19, 0xB8);  // KIK2
    write(0xFF1A, 0x7F);
    write(0xFF1B, 0xFF);
    write(0xFF1C, 0x9F);
    write(0xFF1E, 0xBF);
    write(0xFF20, 0xFF);
    write(0xFF23, 0xBF);
    write(0xFF24, 0x77);
    write(0xFF25, 0xF3);
    write(0xFF26, 0xF1);
    write(0xFF40, 0x91);  // LCDC
    write(0xFF41, 0x85);  // STAT
    write(0xFF46, 0xFF);  // DMA
    write(0xFF47, 0xFC);  // BGP
    write(0xFF48, 0xFF);  // OBJ0
    write(0xFF49, 0xFF);  // OBJ1
    write(0xFF70, 0xFF);  // SVBK
    write(0xFF4F, 0xFF);  // VBK
    write(0xFF4D, 0xFF);  // KEY1
}

void MemoryBus::loadCartridge(std::string filename) {
    std::ifstream infile(filename, std::ios::binary);

    int idx = 0;
    if (infile.is_open()) {
        while (infile) {
            u8 val = infile.get();
            if (idx < 0x20000) {
                cartridge[idx++] = val;
            }
        }
    } else {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }

    //Detect MBC
    switch (cartridge[0x147]) {
        case 1:
        case 2:
        case 3:
            MBC = 1;
            break;
        case 5:
        case 6:
            // MBC = 2;
            break;
        default:
            break;
    }
}

void MemoryBus::write(u16 address, u8 data) {
    //Cartridge ROM Bank
    if (address < 0x8000) {
        HandleBanking(address, data);
    }
    //VRAM
    else if (address < 0xA000) {
        //TODO Fix
        memory[address] = data;
    }
    //Cartridge RAM Bank
    else if (address < 0xC000) {
        ramBanks[address - 0xA000 + (currentRAMBank * 0x2000)] = data;
    }
    //WRAM
    else if (address < 0xE000) {
        memory[address] = data;
    }
    //ECHO Ram
    else if (address < 0xFE00) {
        write(address - 0x2000, data);
    }
    //Sprite Atribute Table (OAM)
    else if (address < 0xFEA0) {
        gpu.writeOAM(address, data);
    }
    //Restricted area
    else if (address < 0xFF00) {
    }
    //I/O Registers
    else if (address < 0xFF80) {
        writeIO(address, data);
    }
    //High RAM
    else if (address < 0xFFFF) {
        memory[address] = data;
    }
    //Interrupt Enable Register
    else {
        interruptManager.setIE(data);
    }
}

u8 MemoryBus::read(u16 address) {
    //Cartridge ROM (Non-Bank)
    if (address < 0x4000) {
        return cartridge[address];
    }
    //Cartridge ROM Bank
    else if (address < 0x8000) {
        return cartridge[address - 0x4000 + (currentROMBank * 0x4000)];
    }
    //VRAM
    else if (address < 0xA000) {
        //TODO Fix
        return memory[address];
    }
    //Cartridge RAM Bank
    else if (address < 0xC000) {
        return ramBanks[address - 0xA000 + (currentRAMBank * 0x2000)];
    }
    //WRAM
    else if (address < 0xE000) {
        return memory[address];
    }
    //ECHO Ram
    else if (address < 0xFE00) {
        return read(address - 0x2000);
    }
    //Sprite Atribute Table (OAM)
    else if (address < 0xFEA0) {
        if (dma.isOamBlocked()) return 0xFF;
        return gpu.readOAM(address);
    }
    //Restricted area
    else if (address < 0xFF00) {
        return 0xFF;
    }
    //I/O Registers
    else if (address < 0xFF80) {
        return readIO(address);
    }
    //High RAM
    else if (address < 0xFFFF) {
        return memory[address];
    }
    //Interrupt Enable Register
    else {
        return interruptManager.getIE();
    }
}

void MemoryBus::writeIO(u16 address, u8 data) {
    switch (address) {
        case 0xFF01:
            memory[address] = data;
            break;
        case 0xFF02:
            memory[address] = data;
            if (data == 0x81) {
                std::cerr << (char)memory[0xFF01];
            }
            break;
        case 0xFF04:
            timer.setDiv();
            break;
        case 0xFF05:
            timer.setTima(data);
            break;
        case 0xFF06:
            timer.setTma(data);
            break;
        case 0xFF07:
            timer.setTac(data);
            break;
        case 0xFF0F:
            interruptManager.setIF(data);
            break;
        default:
            //TODO Fix
            memory[address] = data;
    }
}

u8 MemoryBus::readIO(u16 address) {
    switch (address) {
        case 0xFF01:
            return memory[address];
        case 0xFF02:
            return (memory[address] | 0b01111110);
        case 0xFF04:
            return timer.getDiv();
        case 0xFF05:
            return timer.getTima();
        case 0xFF06:
            return timer.getTma();
        case 0xFF07:
            return timer.getTac();
        case 0xFF0F:
            return interruptManager.getIF();
        default:
            //TODO Fix
            return memory[address];
    }
}

void MemoryBus::HandleBanking(u16 address, u8 data) {
    switch (MBC) {
        case 0:
            return;
        case 1:
            if (address < 0x2000) {
                if (data & 0xF == 0xA)
                    enableRam = 1;
                else
                    enableRam = 0;
            } else if (address < 0x4000) {
                currentROMBank = (currentROMBank & 0xE0) | (data & 0x1F);
            } else if (address < 0x6000) {
                if (romBanking)
                    currentROMBank = (currentROMBank & 0x1F) | ((data & 0x3) << 5);
                else
                    currentRAMBank = (data & 0x3);
            } else {
                romBanking = !(data & 0x1);
                if (romBanking) currentRAMBank = 0;
            }
            break;
        case 2:
        default:
            break;
    }
    if (currentROMBank == 0 || currentROMBank == 0x20 || currentROMBank == 0x40 || currentROMBank == 0x60) currentROMBank++;
}

void MemoryBus::tick() {
    if (!dma.transferInProgress) return;

    if (++dma.ticks < 648) return;

    dma.transferInProgress = 0;
    dma.transferRestarted = 0;
    dma.ticks = 0;

    for (u8 i = 0; i < 0xA0; i++) {
        write((u16)(0xFE00 + i), read(dma.from + i));
    }
}

bool MemoryBus::DMA::isOamBlocked() { return transferRestarted || transferInProgress && ticks >= 5; }

void MemoryBus::DMA::setDMARegister(u8 value) {
    from = (u16)value * 0x100;
    transferRestarted = isOamBlocked();
    ticks = 0;
    transferInProgress = 1;
    DMARegister = value;
}