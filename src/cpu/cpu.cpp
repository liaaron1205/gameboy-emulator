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
            setPair(bc, inc(bc.get()));
            break;
        case 0x04:
            b.set(inc(b.get()));
            break;
        case 0x05:
            b.set(dec(b.get()));
            break;
        case 0x06:
            b.set(fetch());
            break;
        case 0x07:
            a.set(rlc(a.get(), false));
            break;
        case 0x08:
            write16(fetch16(), SP);
            break;
        case 0x09:
            setPair(hl, add(hl.get(), bc.get()));
            break;
        case 0x0A:
            a.set(read(bc.get()));
            break;
        case 0x0B:
            setPair(bc, dec(bc.get()));
            break;
        case 0x0C:
            c.set(inc(c.get()));
            break;
        case 0x0D:
            c.set(dec(c.get()));
            break;
        case 0x0E:
            c.set(fetch());
            break;
        case 0x0F:
            a.set(rrc(a.get(), false));
            break;
        case 0x10:
            //TODO Stop Instruction
            break;
        case 0x11:
            de.set(fetch16());
            break;
        case 0x12:
            write(de.get(), a.get());
            break;
        case 0x13:
            setPair(de, inc(de.get()));
            break;
        case 0x14:
            d.set(inc(d.get()));
            break;
        case 0x15:
            d.set(dec(d.get()));
            break;
        case 0x16:
            d.set(fetch());
            break;
        case 0x17:
            a.set(rl(a.get(), false));
            break;
        case 0x18:
            //TODO JR
            break;
        case 0x19:
            setPair(hl, add(hl.get(), bc.get()));
            break;
        case 0x1A:
            a.set(read(bc.get()));
            break;
        case 0x1B:
            setPair(bc, dec(bc.get()));
            break;
        case 0x1C:
            c.set(inc(c.get()));
            break;
        case 0x1D:
            c.set(dec(c.get()));
            break;
        case 0x1E:
            c.set(fetch());
            break;
        case 0x1F:
            a.set(rr(a.get(), false));
            break;
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

void CPU::setPair(RegisterPair &reg, u16 value) {  // 4 cycles
    reg.set(value);
    cycles += 4;
}
void CPU::setSP(u16 value) {  // 4 cycles
    SP = value;
    cycles += 4;
}
void CPU::setPC(u16 value) {
    PC = value;
    cycles += 4;
}

u8 CPU::inc(u8 value) {
    u8 newValue = value + 1;
    f.setZ(newValue == 0);
    f.setN(0);
    f.setH((value & 0x0F) == 0x0F);
    return newValue;
}
u16 CPU::inc(u16 value) {
    return value + 1;
}
u8 CPU::dec(u8 value) {
    u8 newValue = value - 1;
    f.setZ(newValue == 0);
    f.setN(1);
    f.setH((value & 0x0F) == 0x00);
    return newValue;
}
u16 CPU::dec(u16 value) {
    return value - 1;
}

u8 CPU::rlc(u8 value, bool checkZero) {
    bool bit7 = value & (1 << 7);
    u8 newValue = value << 1;
    if (bit7) newValue |= 1;

    f.setZ(checkZero && (newValue == 0));
    f.setN(0);
    f.setH(0);
    f.setC(bit7);

    return newValue;
}
u8 CPU::rl(u8 value, bool checkZero) {
    bool bit7 = value & (1 << 7);
    u8 newValue = value << 1;
    if (f.getC()) newValue |= 1;

    f.setZ(checkZero && (newValue == 0));
    f.setN(0);
    f.setH(0);
    f.setC(bit7);

    return newValue;
}
u8 CPU::rrc(u8 value, bool checkZero) {
    bool bit0 = value & 1;
    u8 newValue = value >> 1;
    if (bit0) newValue |= (1 << 7);

    f.setZ(checkZero && (newValue == 0));
    f.setN(0);
    f.setH(0);
    f.setC(bit0);

    return newValue;
}
u8 CPU::rr(u8 value, bool checkZero) {
    bool bit0 = value & 1;
    u8 newValue = value >> 1;
    if (f.getC()) newValue |= (1 << 7);

    f.setZ(checkZero && (newValue == 0));
    f.setN(0);
    f.setH(0);
    f.setC(bit0);

    return newValue;
}

u8 CPU::add(u8 term1, u8 term2, bool carry) {
    int carryValue = (carry && f.getC()) ? 1 : 0;
    u8 value = term1 + term2 + carry;

    f.setZ(value == 0);
    f.setN(0);
    f.setH((term1 & 0xF) + (term2 & 0xF) + carryValue > 0xF);
    f.setC((term1 + term2 + carryValue) > 0xFF);

    return value;
}
u16 CPU::add(u16 term1, u16 term2, bool carry) {
    int carryValue = (carry && f.getC()) ? 1 : 0;
    u16 value = term1 + term2 + carry;

    f.setN(0);
    f.setH((term1 & 0xFFF) + (term2 & 0xFFF) + carryValue > 0xFFF);
    f.setC((term1 + term2 + carryValue) > 0xFFFF);

    return value;
}

u8 CPU::sub(u8 term1, u8 term2, bool carry) {
    int carryValue = (carry && f.getC()) ? 1 : 0;
    u8 value = term1 - term2 - carry;

    f.setZ(value == 0);
    f.setN(1);
    f.setH((term1 & 0xF) - (term2 & 0xF) - carryValue < 0);
    f.setC((term1 - term2 - carryValue) < 0);

    return value;
}
u16 CPU::sub(u16 term1, u16 term2, bool carry) {
    int carryValue = (carry && f.getC()) ? 1 : 0;
    u16 value = term1 - term2 - carry;

    f.setN(1);
    f.setH((term1 & 0xFFF) - (term2 & 0xFFF) - carryValue < 0);
    f.setC((term1 - term2 - carryValue) < 0);

    return value;
}
