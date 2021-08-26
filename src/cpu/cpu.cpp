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
            bc.set(inc(bc.get()));
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
            hl.set(add(hl.get(), bc.get()));
            break;
        case 0x0A:
            a.set(read(bc.get()));
            break;
        case 0x0B:
            bc.set(dec(bc.get()));
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
            //TODO Stop
            break;
        case 0x11:
            de.set(fetch16());
            break;
        case 0x12:
            write(de.get(), a.get());
            break;
        case 0x13:
            de.set(inc(de.get()));
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
            jr();
            break;
        case 0x19:
            hl.set(add(hl.get(), de.get()));
            break;
        case 0x1A:
            a.set(read(de.get()));
            break;
        case 0x1B:
            de.set(dec(de.get()));
            break;
        case 0x1C:
            e.set(inc(e.get()));
            break;
        case 0x1D:
            e.set(dec(e.get()));
            break;
        case 0x1E:
            e.set(fetch());
            break;
        case 0x1F:
            a.set(rr(a.get(), false));
            break;
        case 0x20:
            jr(!f.getZ());
            break;
        case 0x21:
            hl.set(fetch16());
            break;
        case 0x22:
            write(hl.pp(), a.get());
            break;
        case 0x23:
            hl.set(inc(hl.get()));
            break;
        case 0x24:
            h.set(inc(h.get()));
            break;
        case 0x25:
            h.set(dec(h.get()));
            break;
        case 0x26:
            h.set(fetch());
            break;
        case 0x27:
            //TODO DAA
            break;
        case 0x28:
            jr(f.getZ());
            break;
        case 0x29:
            hl.set(add(hl.get(), hl.get()));
            break;
        case 0x2A:
            a.set(read(hl.pp()));
            break;
        case 0x2B:
            hl.set(dec(hl.get()));
            break;
        case 0x2C:
            l.set(inc(l.get()));
            break;
        case 0x2D:
            l.set(dec(l.get()));
            break;
        case 0x2E:
            l.set(fetch());
            break;
        case 0x2F:
            //TODO CPL
            break;
        case 0x30:
            jr(!f.getC());
            break;
        case 0x31:
            SP = fetch16();
            break;
        case 0x32:
            write(hl.mm(), a.get());
            break;
        case 0x33:
            SP = inc(SP);
            break;
        case 0x34:
            write(hl.get(), inc(read(hl.get())));
            break;
        case 0x35:
            write(hl.get(), dec(read(hl.get())));
            break;
        case 0x36:
            write(hl.get(), fetch());
            break;
        case 0x37:
            //TODO SCF
            break;
        case 0x38:
            jr(f.getC());
            break;
        case 0x39:
            hl.set(add(hl.get(), SP));
            break;
        case 0x3A:
            a.set(read(hl.mm()));
            break;
        case 0x3B:
            SP = dec(SP);
            break;
        case 0x3C:
            a.set(inc(a.get()));
            break;
        case 0x3D:
            a.set(dec(a.get()));
            break;
        case 0x3E:
            a.set(fetch());
            break;
        case 0x3F:
            //TODO CCF
            break;
        case 0x40:
            b.set(b.get());
            break;
        case 0x41:
            b.set(c.get());
            break;
        case 0x42:
            b.set(d.get());
            break;
        case 0x43:
            b.set(e.get());
            break;
        case 0x44:
            b.set(h.get());
            break;
        case 0x45:
            b.set(l.get());
            break;
        case 0x46:
            b.set(read(hl.get()));
            break;
        case 0x47:
            b.set(a.get());
            break;
        case 0x48:
            c.set(b.get());
            break;
        case 0x49:
            c.set(c.get());
            break;
        case 0x4A:
            c.set(d.get());
            break;
        case 0x4B:
            c.set(e.get());
            break;
        case 0x4C:
            c.set(h.get());
            break;
        case 0x4D:
            c.set(l.get());
            break;
        case 0x4E:
            c.set(read(hl.get()));
            break;
        case 0x4F:
            c.set(a.get());
            break;
        case 0x50:
            d.set(b.get());
            break;
        case 0x51:
            d.set(c.get());
            break;
        case 0x52:
            d.set(d.get());
            break;
        case 0x53:
            d.set(e.get());
            break;
        case 0x54:
            d.set(h.get());
            break;
        case 0x55:
            d.set(l.get());
            break;
        case 0x56:
            d.set(read(hl.get()));
            break;
        case 0x57:
            d.set(a.get());
            break;
        case 0x58:
            e.set(b.get());
            break;
        case 0x59:
            e.set(c.get());
            break;
        case 0x5A:
            e.set(d.get());
            break;
        case 0x5B:
            e.set(e.get());
            break;
        case 0x5C:
            e.set(h.get());
            break;
        case 0x5D:
            e.set(l.get());
            break;
        case 0x5E:
            e.set(read(hl.get()));
            break;
        case 0x5F:
            e.set(a.get());
            break;
        case 0x60:
            h.set(b.get());
            break;
        case 0x61:
            h.set(c.get());
            break;
        case 0x62:
            h.set(d.get());
            break;
        case 0x63:
            h.set(e.get());
            break;
        case 0x64:
            h.set(h.get());
            break;
        case 0x65:
            h.set(l.get());
            break;
        case 0x66:
            h.set(read(hl.get()));
            break;
        case 0x67:
            h.set(a.get());
            break;
        case 0x68:
            l.set(b.get());
            break;
        case 0x69:
            l.set(c.get());
            break;
        case 0x6A:
            l.set(d.get());
            break;
        case 0x6B:
            l.set(e.get());
            break;
        case 0x6C:
            l.set(h.get());
            break;
        case 0x6D:
            l.set(l.get());
            break;
        case 0x6E:
            l.set(read(hl.get()));
            break;
        case 0x6F:
            l.set(a.get());
            break;
        case 0x70:
            write(hl.get(), b.get());
            break;
        case 0x71:
            write(hl.get(), c.get());
            break;
        case 0x72:
            write(hl.get(), d.get());
            break;
        case 0x73:
            write(hl.get(), e.get());
            break;
        case 0x74:
            write(hl.get(), h.get());
            break;
        case 0x75:
            write(hl.get(), l.get());
            break;
        case 0x76:
            //TODO Halt
            break;
        case 0x77:
            write(hl.get(), a.get());
            break;
        case 0x78:
            a.set(b.get());
            break;
        case 0x79:
            a.set(c.get());
            break;
        case 0x7A:
            a.set(d.get());
            break;
        case 0x7B:
            a.set(e.get());
            break;
        case 0x7C:
            a.set(h.get());
            break;
        case 0x7D:
            a.set(l.get());
            break;
        case 0x7E:
            a.set(read(hl.get()));
            break;
        case 0x7F:
            a.set(a.get());
            break;
        case 0x80:
            a.set(add(a.get(), b.get()));
            break;
        case 0x81:
            a.set(add(a.get(), c.get()));
            break;
        case 0x82:
            a.set(add(a.get(), d.get()));
            break;
        case 0x83:
            a.set(add(a.get(), e.get()));
            break;
        case 0x84:
            a.set(add(a.get(), h.get()));
            break;
        case 0x85:
            a.set(add(a.get(), l.get()));
            break;
        case 0x86:
            a.set(add(a.get(), read(hl.get())));
            break;
        case 0x87:
            a.set(add(a.get(), a.get()));
            break;
        case 0x88:
            a.set(add(a.get(), b.get(), true));
            break;
        case 0x89:
            a.set(add(a.get(), c.get(), true));
            break;
        case 0x8A:
            a.set(add(a.get(), d.get(), true));
            break;
        case 0x8B:
            a.set(add(a.get(), e.get(), true));
            break;
        case 0x8C:
            a.set(add(a.get(), h.get(), true));
            break;
        case 0x8D:
            a.set(add(a.get(), l.get(), true));
            break;
        case 0x8E:
            a.set(add(a.get(), read(hl.get()), true));
            break;
        case 0x8F:
            a.set(add(a.get(), a.get(), true));
            break;
        case 0x90:
            a.set(sub(a.get(), b.get()));
            break;
        case 0x91:
            a.set(sub(a.get(), c.get()));
            break;
        case 0x92:
            a.set(sub(a.get(), d.get()));
            break;
        case 0x93:
            a.set(sub(a.get(), e.get()));
            break;
        case 0x94:
            a.set(sub(a.get(), h.get()));
            break;
        case 0x95:
            a.set(sub(a.get(), l.get()));
            break;
        case 0x96:
            a.set(sub(a.get(), read(hl.get())));
            break;
        case 0x97:
            a.set(sub(a.get(), a.get()));
            break;
        case 0x98:
            a.set(sub(a.get(), b.get(), true));
            break;
        case 0x99:
            a.set(sub(a.get(), c.get(), true));
            break;
        case 0x9A:
            a.set(sub(a.get(), d.get(), true));
            break;
        case 0x9B:
            a.set(sub(a.get(), e.get(), true));
            break;
        case 0x9C:
            a.set(sub(a.get(), h.get(), true));
            break;
        case 0x9D:
            a.set(sub(a.get(), l.get(), true));
            break;
        case 0x9E:
            a.set(sub(a.get(), read(hl.get()), true));
            break;
        case 0x9F:
            a.set(sub(a.get(), a.get(), true));
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
    u8 lo = read(address);
    u8 hi = read(address + 1);
    return (u16)lo | ((u16)hi << 8);
}
void CPU::write(u16 address, u8 value) {  // 4 cycles
    cycles += 4;
    memory.write(address, value);
}
void CPU::write16(u16 address, u16 value) {  // 8 cycles
    u8 lo = (u8)(value & 0xFF);
    u8 hi = (u8)((value & 0xFF00) >> 8);
    write(address, lo);
    write(address + 1, hi);
}

u8 CPU::fetch() { return read(PC++); }  // 4 cycles
u16 CPU::fetch16() {                    // 8 cycles
    u16 value = read16(PC);
    PC += 2;
    return value;
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
u16 CPU::inc(u16 value) {  // 4 cycles
    cycles += 4;
    return value + 1;
}
u8 CPU::dec(u8 value) {
    u8 newValue = value - 1;
    f.setZ(newValue == 0);
    f.setN(1);
    f.setH((value & 0x0F) == 0x00);
    return newValue;
}
u16 CPU::dec(u16 value) {  // 4 cycles
    cycles += 4;
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
    u8 value = term1 + term2 + carryValue;

    f.setZ(value == 0);
    f.setN(0);
    f.setH((term1 & 0xF) + (term2 & 0xF) + carryValue > 0xF);
    f.setC((term1 + term2 + carryValue) > 0xFF);

    return value;
}
u16 CPU::add(u16 term1, u16 term2, bool carry) {
    int carryValue = (carry && f.getC()) ? 1 : 0;
    u16 value = term1 + term2 + carryValue;

    f.setN(0);
    f.setH((term1 & 0xFFF) + (term2 & 0xFFF) + carryValue > 0xFFF);
    f.setC((term1 + term2 + carryValue) > 0xFFFF);

    return value;
}

u8 CPU::sub(u8 term1, u8 term2, bool carry) {
    int carryValue = (carry && f.getC()) ? 1 : 0;
    u8 value = term1 - term2 - carryValue;

    f.setZ(value == 0);
    f.setN(1);
    f.setH((term1 & 0xF) - (term2 & 0xF) - carryValue < 0);
    f.setC((term1 - term2 - carryValue) < 0);

    return value;
}
u16 CPU::sub(u16 term1, u16 term2, bool carry) {
    int carryValue = (carry && f.getC()) ? 1 : 0;
    u16 value = term1 - term2 - carryValue;

    f.setN(1);
    f.setH((term1 & 0xFFF) - (term2 & 0xFFF) - carryValue < 0);
    f.setC((term1 - term2 - carryValue) < 0);

    return value;
}

void CPU::jr(bool cond) {
    u16 newPC = extend(fetch()) + PC - 2;
    if (cond) setPC(newPC);
}

u16 extend(u8 value) {
    return (u16)(i16)(i8)value;
}