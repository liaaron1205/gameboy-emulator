#include "cpu.h"

CPU::CPU() {
    af.set(0x01B);
    bc.set(0x0013);
    de.set(0x00D8);
    hl.set(0x014D);
}

void CPU::fetchDecodeExecute() {
    u8 opcode = fetch();
    switch (opcode) {
        case 0x00:
            break;
        case 0x01:
            bc.set(fetch16());
            break;
        case 0x02:
            write(bc.get(), a.get());
            break;
        case 0x03:

        default:
            break;
    }
}
int CPU::getCycles() { return cycles; }

u8 CPU::read(u16 address) {  // 4 cycles
    cycles += 4;
    return memory.read(address);
}
u16 CPU::read16(u16 address) {  // 8 cycles
    cycles += 8;
    u8 lo = memory.read(address);
    u8 hi = memory.read(address + 1);
    return (u16)lo | ((u16)hi << 8);
}

void CPU::write(u16 address, u8 value) {  // 4 cycles
    cycles += 4;
    memory.write(address, value);
}
void CPU::write16(u16 address, u16 value) {  // 8 cycles
    cycles += 8;
    u8 lo = (u8)(value & 0xFF);
    u8 hi = (u8)((value & 0xFF00) >> 8);
    memory.write(address, lo);
    memory.write(address + 1, hi);
}

u8 CPU::fetch() { return read(PC++); }  // 4 cycles
u16 CPU::fetch16() {                    // 8 cycles
    u16 value = read16(PC);
    PC += 2;
    return value;
}
