#include "cpu.h"

CPU::CPU(MemoryBus& memory, InterruptManager& interruptManager) : memory{memory}, interruptManager{interruptManager} {
    af.set(0x01B);
    bc.set(0x0013);
    de.set(0x00D8);
    hl.set(0x014D);

    PC = 0x100;
    SP = 0xFFEE;
}

void CPU::tick() {
    if (cycles == 0) {
        u16 address = interruptManager.checkInterrupts();
        serviceInterrupts(address);
    }
    if (cycles == 0) fetchDecodeExecute();
    cycles--;
}

void CPU::serviceInterrupts(u16 address) {
    if (address == 0xFFFF) return;
    cycles += 8;
    push(PC);
    setPC(address);
}

void CPU::fetchDecodeExecute() {
    // printDebug();
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
            a.set(rlc(a.get(), 0));
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
            a.set(rrc(a.get(), 0));
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
            a.set(rl(a.get(), 0));
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
            a.set(rr(a.get(), 0));
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
            if (f.getN()) {  //subtraction
                if (f.getC()) a.set(a.get() - 0x60);
                if (f.getH()) a.set(a.get() - 0x06);

            } else {
                if (f.getC() || (a.get() & 0xFF) > 0x99) {
                    a.set(a.get() + 0x60);
                    f.setC(1);
                }
                if (f.getH() || (a.get() & 0x0F) > 0x09) a.set(a.get() + 0x06);
            }
            f.setZ(a.get() == 0);
            f.setH(0);
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
            a.set(~a.get());
            f.setN(1);
            f.setH(1);
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
            f.setN(0);
            f.setH(0);
            f.setC(1);
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
            f.setN(0);
            f.setH(0);
            f.setC(!f.getC());
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
            printf("halt called\n");
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
            a.set(add(a.get(), b.get(), 1));
            break;
        case 0x89:
            a.set(add(a.get(), c.get(), 1));
            break;
        case 0x8A:
            a.set(add(a.get(), d.get(), 1));
            break;
        case 0x8B:
            a.set(add(a.get(), e.get(), 1));
            break;
        case 0x8C:
            a.set(add(a.get(), h.get(), 1));
            break;
        case 0x8D:
            a.set(add(a.get(), l.get(), 1));
            break;
        case 0x8E:
            a.set(add(a.get(), read(hl.get()), 1));
            break;
        case 0x8F:
            a.set(add(a.get(), a.get(), 1));
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
            a.set(sub(a.get(), b.get(), 1));
            break;
        case 0x99:
            a.set(sub(a.get(), c.get(), 1));
            break;
        case 0x9A:
            a.set(sub(a.get(), d.get(), 1));
            break;
        case 0x9B:
            a.set(sub(a.get(), e.get(), 1));
            break;
        case 0x9C:
            a.set(sub(a.get(), h.get(), 1));
            break;
        case 0x9D:
            a.set(sub(a.get(), l.get(), 1));
            break;
        case 0x9E:
            a.set(sub(a.get(), read(hl.get()), 1));
            break;
        case 0x9F:
            a.set(sub(a.get(), a.get(), 1));
            break;
        case 0xA0:
            a.set(land(a.get(), b.get()));
            break;
        case 0xA1:
            a.set(land(a.get(), c.get()));
            break;
        case 0xA2:
            a.set(land(a.get(), d.get()));
            break;
        case 0xA3:
            a.set(land(a.get(), e.get()));
            break;
        case 0xA4:
            a.set(land(a.get(), h.get()));
            break;
        case 0xA5:
            a.set(land(a.get(), l.get()));
            break;
        case 0xA6:
            a.set(land(a.get(), read(hl.get())));
            break;
        case 0xA7:
            a.set(land(a.get(), a.get()));
            break;
        case 0xA8:
            a.set(lxor(a.get(), b.get()));
            break;
        case 0xA9:
            a.set(lxor(a.get(), c.get()));
            break;
        case 0xAA:
            a.set(lxor(a.get(), d.get()));
            break;
        case 0xAB:
            a.set(lxor(a.get(), e.get()));
            break;
        case 0xAC:
            a.set(lxor(a.get(), h.get()));
            break;
        case 0xAD:
            a.set(lxor(a.get(), l.get()));
            break;
        case 0xAE:
            a.set(lxor(a.get(), read(hl.get())));
            break;
        case 0xAF:
            a.set(lxor(a.get(), a.get()));
            break;
        case 0xB0:
            a.set(lor(a.get(), b.get()));
            break;
        case 0xB1:
            a.set(lor(a.get(), c.get()));
            break;
        case 0xB2:
            a.set(lor(a.get(), d.get()));
            break;
        case 0xB3:
            a.set(lor(a.get(), e.get()));
            break;
        case 0xB4:
            a.set(lor(a.get(), h.get()));
            break;
        case 0xB5:
            a.set(lor(a.get(), l.get()));
            break;
        case 0xB6:
            a.set(lor(a.get(), read(hl.get())));
            break;
        case 0xB7:
            a.set(lor(a.get(), a.get()));
            break;
        case 0xB8:
            cp(a.get(), b.get());
            break;
        case 0xB9:
            cp(a.get(), c.get());
            break;
        case 0xBA:
            cp(a.get(), d.get());
            break;
        case 0xBB:
            cp(a.get(), e.get());
            break;
        case 0xBC:
            cp(a.get(), h.get());
            break;
        case 0xBD:
            cp(a.get(), l.get());
            break;
        case 0xBE:
            cp(a.get(), read(hl.get()));
            break;
        case 0xBF:
            cp(a.get(), a.get());
            break;
        case 0xC0:
            ret(!f.getZ());
            break;
        case 0xC1:
            bc.set(pop());
            break;
        case 0xC2:
            jp(!f.getZ());
            break;
        case 0xC3:
            jp();
            break;
        case 0xC4:
            call(!f.getZ());
            break;
        case 0xC5:
            pushOP(bc.get());
            break;
        case 0xC6:
            a.set(add(a.get(), fetch()));
            break;
        case 0xC7:
            rst(0x00);
            break;
        case 0xC8:
            ret(f.getZ());
            break;
        case 0xC9:
            ret();
            break;
        case 0xCA:
            jp(f.getZ());
            break;
        case 0xCB:
            fetchDecodeExecutePrefix();
            break;
        case 0xCC:
            call(f.getZ());
            break;
        case 0xCD:
            call(1);
            break;
        case 0xCE:
            a.set(add(a.get(), fetch(), 1));
            break;
        case 0xCF:
            rst(0x08);
            break;
        case 0xD0:
            ret(!f.getC());
            break;
        case 0xD1:
            de.set(pop());
            break;
        case 0xD2:
            jp(!f.getC());
            break;
        case 0xD4:
            call(!f.getC());
            break;
        case 0xD5:
            pushOP(de.get());
            break;
        case 0xD6:
            a.set(sub(a.get(), fetch()));
            break;
        case 0xD7:
            rst(0x10);
            break;
        case 0xD8:
            ret(f.getC());
            break;
        case 0xD9:
            ret();
            interruptManager.setIME(1);
            break;
        case 0xDA:
            jp(f.getC());
            break;
        case 0xDC:
            call(f.getC());
            break;
        case 0xDE:
            a.set(sub(a.get(), fetch(), 1));
            break;
        case 0xDF:
            rst(0x18);
            break;
        case 0xE0:
            write(0xFF00 + fetch(), a.get());
            break;
        case 0xE1:
            hl.set(pop());
            break;
        case 0xE2:
            write(0xFF00 + c.get(), a.get());
            break;
        case 0xE5:
            pushOP(hl.get());
            break;
        case 0xE6:
            a.set(land(a.get(), fetch()));
            break;
        case 0xE7:
            rst(0x20);
            break;
        case 0xE8:
            SP = add(SP, fetch());
            cycles += 4;
            break;
        case 0xE9:
            setPC(hl.get());
            cycles -= 4;
            break;
        case 0xEA:
            write(fetch16(), a.get());
            break;
        case 0xEE:
            a.set(lxor(a.get(), fetch()));
            break;
        case 0xEF:
            rst(0x28);
            break;
        case 0xF0:
            a.set(read(0xFF00 + fetch()));
            break;
        case 0xF1:
            af.set(pop());
            f.clearLowBits();
            break;
        case 0xF2:
            a.set(read(0xFF00 + c.get()));
            break;
        case 0xF3:
            interruptManager.setIME(0);
            break;
        case 0xF5:
            pushOP(af.get());
            break;
        case 0xF6:
            a.set(lor(a.get(), fetch()));
            break;
        case 0xF7:
            rst(0x30);
            break;
        case 0xF8:
            hl.set(add(SP, fetch()));
            break;
        case 0xF9:
            SP = hl.get();
            cycles += 4;
            break;
        case 0xFA:
            a.set(read(fetch16()));
            break;
        case 0xFB:
            interruptManager.setIME(1);
            break;
        case 0xFE:
            cp(a.get(), fetch());
            break;
        case 0xFF:
            rst(0x38);
            break;
        default:
            break;
    }
}
void CPU::fetchDecodeExecutePrefix() {
    u8 opcode = fetch();
    switch (opcode) {
        case 0x00:
            b.set(rlc(b.get()));
            break;
        case 0x01:
            c.set(rlc(c.get()));
            break;
        case 0x02:
            d.set(rlc(d.get()));
            break;
        case 0x03:
            e.set(rlc(e.get()));
            break;
        case 0x04:
            h.set(rlc(h.get()));
            break;
        case 0x05:
            l.set(rlc(l.get()));
            break;
        case 0x06:
            write(hl.get(), rlc(read(hl.get())));
            break;
        case 0x07:
            a.set(rlc(a.get()));
            break;
        case 0x08:
            b.set(rrc(b.get()));
            break;
        case 0x09:
            c.set(rrc(c.get()));
            break;
        case 0x0A:
            d.set(rrc(d.get()));
            break;
        case 0x0B:
            e.set(rrc(e.get()));
            break;
        case 0x0C:
            h.set(rrc(h.get()));
            break;
        case 0x0D:
            l.set(rrc(l.get()));
            break;
        case 0x0E:
            write(hl.get(), rrc(read(hl.get())));
            break;
        case 0x0F:
            a.set(rrc(a.get()));
            break;
        case 0x10:
            b.set(rl(b.get()));
            break;
        case 0x11:
            c.set(rl(c.get()));
            break;
        case 0x12:
            d.set(rl(d.get()));
            break;
        case 0x13:
            e.set(rl(e.get()));
            break;
        case 0x14:
            h.set(rl(h.get()));
            break;
        case 0x15:
            l.set(rl(l.get()));
            break;
        case 0x16:
            write(hl.get(), rl(read(hl.get())));
            break;
        case 0x17:
            a.set(rl(a.get()));
            break;
        case 0x18:
            b.set(rr(b.get()));
            break;
        case 0x19:
            c.set(rr(c.get()));
            break;
        case 0x1A:
            d.set(rr(d.get()));
            break;
        case 0x1B:
            e.set(rr(e.get()));
            break;
        case 0x1C:
            h.set(rr(h.get()));
            break;
        case 0x1D:
            l.set(rr(l.get()));
            break;
        case 0x1E:
            write(hl.get(), rr(read(hl.get())));
            break;
        case 0x1F:
            a.set(rr(a.get()));
            break;
        case 0x20:
            b.set(sla(b.get()));
            break;
        case 0x21:
            c.set(sla(c.get()));
            break;
        case 0x22:
            d.set(sla(d.get()));
            break;
        case 0x23:
            e.set(sla(e.get()));
            break;
        case 0x24:
            h.set(sla(h.get()));
            break;
        case 0x25:
            l.set(sla(l.get()));
            break;
        case 0x26:
            write(hl.get(), sla(read(hl.get())));
            break;
        case 0x27:
            a.set(sla(a.get()));
            break;
        case 0x28:
            b.set(sra(b.get()));
            break;
        case 0x29:
            c.set(sra(c.get()));
            break;
        case 0x2A:
            d.set(sra(d.get()));
            break;
        case 0x2B:
            e.set(sra(e.get()));
            break;
        case 0x2C:
            h.set(sra(h.get()));
            break;
        case 0x2D:
            l.set(sra(l.get()));
            break;
        case 0x2E:
            write(hl.get(), sra(read(hl.get())));
            break;
        case 0x2F:
            a.set(sra(a.get()));
            break;
        case 0x30:
            b.set(swap(b.get()));
            break;
        case 0x31:
            c.set(swap(c.get()));
            break;
        case 0x32:
            d.set(swap(d.get()));
            break;
        case 0x33:
            e.set(swap(e.get()));
            break;
        case 0x34:
            h.set(swap(h.get()));
            break;
        case 0x35:
            l.set(swap(l.get()));
            break;
        case 0x36:
            write(hl.get(), swap(read(hl.get())));
            break;
        case 0x37:
            a.set(swap(a.get()));
            break;
        case 0x38:
            b.set(srl(b.get()));
            break;
        case 0x39:
            c.set(srl(c.get()));
            break;
        case 0x3A:
            d.set(srl(d.get()));
            break;
        case 0x3B:
            e.set(srl(e.get()));
            break;
        case 0x3C:
            h.set(srl(h.get()));
            break;
        case 0x3D:
            l.set(srl(l.get()));
            break;
        case 0x3E:
            write(hl.get(), srl(read(hl.get())));
            break;
        case 0x3F:
            a.set(srl(a.get()));
            break;
        case 0x40:
            bit(0, b.get());
            break;
        case 0x41:
            bit(0, c.get());
            break;
        case 0x42:
            bit(0, d.get());
            break;
        case 0x43:
            bit(0, e.get());
            break;
        case 0x44:
            bit(0, h.get());
            break;
        case 0x45:
            bit(0, l.get());
            break;
        case 0x46:
            bit(0, read(hl.get()));
            break;
        case 0x47:
            bit(0, a.get());
            break;
        case 0x48:
            bit(1, b.get());
            break;
        case 0x49:
            bit(1, c.get());
            break;
        case 0x4A:
            bit(1, d.get());
            break;
        case 0x4B:
            bit(1, e.get());
            break;
        case 0x4C:
            bit(1, h.get());
            break;
        case 0x4D:
            bit(1, l.get());
            break;
        case 0x4E:
            bit(1, read(hl.get()));
            break;
        case 0x4F:
            bit(1, a.get());
            break;
        case 0x50:
            bit(2, b.get());
            break;
        case 0x51:
            bit(2, c.get());
            break;
        case 0x52:
            bit(2, d.get());
            break;
        case 0x53:
            bit(2, e.get());
            break;
        case 0x54:
            bit(2, h.get());
            break;
        case 0x55:
            bit(2, l.get());
            break;
        case 0x56:
            bit(2, read(hl.get()));
            break;
        case 0x57:
            bit(2, a.get());
            break;
        case 0x58:
            bit(3, b.get());
            break;
        case 0x59:
            bit(3, c.get());
            break;
        case 0x5A:
            bit(3, d.get());
            break;
        case 0x5B:
            bit(3, e.get());
            break;
        case 0x5C:
            bit(3, h.get());
            break;
        case 0x5D:
            bit(3, l.get());
            break;
        case 0x5E:
            bit(3, read(hl.get()));
            break;
        case 0x5F:
            bit(3, a.get());
            break;
        case 0x60:
            bit(4, b.get());
            break;
        case 0x61:
            bit(4, c.get());
            break;
        case 0x62:
            bit(4, d.get());
            break;
        case 0x63:
            bit(4, e.get());
            break;
        case 0x64:
            bit(4, h.get());
            break;
        case 0x65:
            bit(4, l.get());
            break;
        case 0x66:
            bit(4, read(hl.get()));
            break;
        case 0x67:
            bit(4, a.get());
            break;
        case 0x68:
            bit(5, b.get());
            break;
        case 0x69:
            bit(5, c.get());
            break;
        case 0x6A:
            bit(5, d.get());
            break;
        case 0x6B:
            bit(5, e.get());
            break;
        case 0x6C:
            bit(5, h.get());
            break;
        case 0x6D:
            bit(5, l.get());
            break;
        case 0x6E:
            bit(5, read(hl.get()));
            break;
        case 0x6F:
            bit(5, a.get());
            break;
        case 0x70:
            bit(6, b.get());
            break;
        case 0x71:
            bit(6, c.get());
            break;
        case 0x72:
            bit(6, d.get());
            break;
        case 0x73:
            bit(6, e.get());
            break;
        case 0x74:
            bit(6, h.get());
            break;
        case 0x75:
            bit(6, l.get());
            break;
        case 0x76:
            bit(6, read(hl.get()));
            break;
        case 0x77:
            bit(6, a.get());
            break;
        case 0x78:
            bit(7, b.get());
            break;
        case 0x79:
            bit(7, c.get());
            break;
        case 0x7A:
            bit(7, d.get());
            break;
        case 0x7B:
            bit(7, e.get());
            break;
        case 0x7C:
            bit(7, h.get());
            break;
        case 0x7D:
            bit(7, l.get());
            break;
        case 0x7E:
            bit(7, read(hl.get()));
            break;
        case 0x7F:
            bit(7, a.get());
            break;
        case 0x80:
            b.set(res(0, b.get()));
            break;
        case 0x81:
            c.set(res(0, c.get()));
            break;
        case 0x82:
            d.set(res(0, d.get()));
            break;
        case 0x83:
            e.set(res(0, e.get()));
            break;
        case 0x84:
            h.set(res(0, h.get()));
            break;
        case 0x85:
            l.set(res(0, l.get()));
            break;
        case 0x86:
            write(hl.get(), res(0, read(hl.get())));
            break;
        case 0x87:
            a.set(res(0, a.get()));
            break;
        case 0x88:
            b.set(res(1, b.get()));
            break;
        case 0x89:
            c.set(res(1, c.get()));
            break;
        case 0x8A:
            d.set(res(1, d.get()));
            break;
        case 0x8B:
            e.set(res(1, e.get()));
            break;
        case 0x8C:
            h.set(res(1, h.get()));
            break;
        case 0x8D:
            l.set(res(1, l.get()));
            break;
        case 0x8E:
            write(hl.get(), res(1, read(hl.get())));
            break;
        case 0x8F:
            a.set(res(1, a.get()));
            break;
        case 0x90:
            b.set(res(2, b.get()));
            break;
        case 0x91:
            c.set(res(2, c.get()));
            break;
        case 0x92:
            d.set(res(2, d.get()));
            break;
        case 0x93:
            e.set(res(2, e.get()));
            break;
        case 0x94:
            h.set(res(2, h.get()));
            break;
        case 0x95:
            l.set(res(2, l.get()));
            break;
        case 0x96:
            write(hl.get(), res(2, read(hl.get())));
            break;
        case 0x97:
            a.set(res(2, a.get()));
            break;
        case 0x98:
            b.set(res(3, b.get()));
            break;
        case 0x99:
            c.set(res(3, c.get()));
            break;
        case 0x9A:
            d.set(res(3, d.get()));
            break;
        case 0x9B:
            e.set(res(3, e.get()));
            break;
        case 0x9C:
            h.set(res(3, h.get()));
            break;
        case 0x9D:
            l.set(res(3, l.get()));
            break;
        case 0x9E:
            write(hl.get(), res(3, read(hl.get())));
            break;
        case 0x9F:
            a.set(res(3, a.get()));
            break;
        case 0xA0:
            b.set(res(4, b.get()));
            break;
        case 0xA1:
            c.set(res(4, c.get()));
            break;
        case 0xA2:
            d.set(res(4, d.get()));
            break;
        case 0xA3:
            e.set(res(4, e.get()));
            break;
        case 0xA4:
            h.set(res(4, h.get()));
            break;
        case 0xA5:
            l.set(res(4, l.get()));
            break;
        case 0xA6:
            write(hl.get(), res(4, read(hl.get())));
            break;
        case 0xA7:
            a.set(res(4, a.get()));
            break;
        case 0xA8:
            b.set(res(5, b.get()));
            break;
        case 0xA9:
            c.set(res(5, c.get()));
            break;
        case 0xAA:
            d.set(res(5, d.get()));
            break;
        case 0xAB:
            e.set(res(5, e.get()));
            break;
        case 0xAC:
            h.set(res(5, h.get()));
            break;
        case 0xAD:
            l.set(res(5, l.get()));
            break;
        case 0xAE:
            write(hl.get(), res(5, read(hl.get())));
            break;
        case 0xAF:
            a.set(res(5, a.get()));
            break;
        case 0xB0:
            b.set(res(6, b.get()));
            break;
        case 0xB1:
            c.set(res(6, c.get()));
            break;
        case 0xB2:
            d.set(res(6, d.get()));
            break;
        case 0xB3:
            e.set(res(6, e.get()));
            break;
        case 0xB4:
            h.set(res(6, h.get()));
            break;
        case 0xB5:
            l.set(res(6, l.get()));
            break;
        case 0xB6:
            write(hl.get(), res(6, read(hl.get())));
            break;
        case 0xB7:
            a.set(res(6, a.get()));
            break;
        case 0xB8:
            b.set(res(7, b.get()));
            break;
        case 0xB9:
            c.set(res(7, c.get()));
            break;
        case 0xBA:
            d.set(res(7, d.get()));
            break;
        case 0xBB:
            e.set(res(7, e.get()));
            break;
        case 0xBC:
            h.set(res(7, h.get()));
            break;
        case 0xBD:
            l.set(res(7, l.get()));
            break;
        case 0xBE:
            write(hl.get(), res(7, read(hl.get())));
            break;
        case 0xBF:
            a.set(res(7, a.get()));
            break;
        case 0xC0:
            b.set(set(0, b.get()));
            break;
        case 0xC1:
            c.set(set(0, c.get()));
            break;
        case 0xC2:
            d.set(set(0, d.get()));
            break;
        case 0xC3:
            e.set(set(0, e.get()));
            break;
        case 0xC4:
            h.set(set(0, h.get()));
            break;
        case 0xC5:
            l.set(set(0, l.get()));
            break;
        case 0xC6:
            write(hl.get(), set(0, read(hl.get())));
            break;
        case 0xC7:
            a.set(set(0, a.get()));
            break;
        case 0xC8:
            b.set(set(1, b.get()));
            break;
        case 0xC9:
            c.set(set(1, c.get()));
            break;
        case 0xCA:
            d.set(set(1, d.get()));
            break;
        case 0xCB:
            e.set(set(1, e.get()));
            break;
        case 0xCC:
            h.set(set(1, h.get()));
            break;
        case 0xCD:
            l.set(set(1, l.get()));
            break;
        case 0xCE:
            write(hl.get(), set(1, read(hl.get())));
            break;
        case 0xCF:
            a.set(set(1, a.get()));
            break;
        case 0xD0:
            b.set(set(2, b.get()));
            break;
        case 0xD1:
            c.set(set(2, c.get()));
            break;
        case 0xD2:
            d.set(set(2, d.get()));
            break;
        case 0xD3:
            e.set(set(2, e.get()));
            break;
        case 0xD4:
            h.set(set(2, h.get()));
            break;
        case 0xD5:
            l.set(set(2, l.get()));
            break;
        case 0xD6:
            write(hl.get(), set(2, read(hl.get())));
            break;
        case 0xD7:
            a.set(set(2, a.get()));
            break;
        case 0xD8:
            b.set(set(3, b.get()));
            break;
        case 0xD9:
            c.set(set(3, c.get()));
            break;
        case 0xDA:
            d.set(set(3, d.get()));
            break;
        case 0xDB:
            e.set(set(3, e.get()));
            break;
        case 0xDC:
            h.set(set(3, h.get()));
            break;
        case 0xDD:
            l.set(set(3, l.get()));
            break;
        case 0xDE:
            write(hl.get(), set(3, read(hl.get())));
            break;
        case 0xDF:
            a.set(set(3, a.get()));
            break;
        case 0xE0:
            b.set(set(4, b.get()));
            break;
        case 0xE1:
            c.set(set(4, c.get()));
            break;
        case 0xE2:
            d.set(set(4, d.get()));
            break;
        case 0xE3:
            e.set(set(4, e.get()));
            break;
        case 0xE4:
            h.set(set(4, h.get()));
            break;
        case 0xE5:
            l.set(set(4, l.get()));
            break;
        case 0xE6:
            write(hl.get(), set(4, read(hl.get())));
            break;
        case 0xE7:
            a.set(set(4, a.get()));
            break;
        case 0xE8:
            b.set(set(5, b.get()));
            break;
        case 0xE9:
            c.set(set(5, c.get()));
            break;
        case 0xEA:
            d.set(set(5, d.get()));
            break;
        case 0xEB:
            e.set(set(5, e.get()));
            break;
        case 0xEC:
            h.set(set(5, h.get()));
            break;
        case 0xED:
            l.set(set(5, l.get()));
            break;
        case 0xEE:
            write(hl.get(), set(5, read(hl.get())));
            break;
        case 0xEF:
            a.set(set(5, a.get()));
            break;
        case 0xF0:
            b.set(set(6, b.get()));
            break;
        case 0xF1:
            c.set(set(6, c.get()));
            break;
        case 0xF2:
            d.set(set(6, d.get()));
            break;
        case 0xF3:
            e.set(set(6, e.get()));
            break;
        case 0xF4:
            h.set(set(6, h.get()));
            break;
        case 0xF5:
            l.set(set(6, l.get()));
            break;
        case 0xF6:
            write(hl.get(), set(6, read(hl.get())));
            break;
        case 0xF7:
            a.set(set(6, a.get()));
            break;
        case 0xF8:
            b.set(set(7, b.get()));
            break;
        case 0xF9:
            c.set(set(7, c.get()));
            break;
        case 0xFA:
            d.set(set(7, d.get()));
            break;
        case 0xFB:
            e.set(set(7, e.get()));
            break;
        case 0xFC:
            h.set(set(7, h.get()));
            break;
        case 0xFD:
            l.set(set(7, l.get()));
            break;
        case 0xFE:
            write(hl.get(), set(7, read(hl.get())));
            break;
        case 0xFF:
            a.set(set(7, a.get()));
            break;
    }
}

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

void CPU::setPC(u16 value) {  // 4 cycles
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
u8 CPU::sla(u8 value) {
    bool bit7 = value & (1 << 7);
    u8 newValue = value << 1;

    f.setZ(newValue == 0);
    f.setN(0);
    f.setH(0);
    f.setC(bit7);

    return newValue;
}
u8 CPU::sra(u8 value) {
    bool bit0 = value & 1;
    u8 bit7 = value & (1 << 7);
    u8 newValue = value >> 1;
    newValue |= bit7;

    f.setZ(newValue == 0);
    f.setN(0);
    f.setH(0);
    f.setC(bit0);

    return newValue;
}
u8 CPU::srl(u8 value) {
    bool bit0 = value & 1;
    u8 newValue = value >> 1;

    f.setZ(newValue == 0);
    f.setN(0);
    f.setH(0);
    f.setC(bit0);

    return newValue;
}
u8 CPU::swap(u8 value) {
    u8 lo = value & 0x0F;
    u8 hi = value & 0xF0;

    u8 newValue = (hi >> 4) | (lo << 4);

    f.setZ(newValue == 0);
    f.setN(0);
    f.setH(0);
    f.setC(0);

    return newValue;
}

void CPU::bit(int num, u8 value) {
    bool isSet = (value & (1 << num));
    f.setZ(!isSet);
    f.setN(0);
    f.setH(1);
}
u8 CPU::res(int num, u8 value) {
    return value & ~(1 << num);
}
u8 CPU::set(int num, u8 value) {
    return value | (1 << num);
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
u16 CPU::add(u16 term1, u16 term2, bool carry) {  //4 extra cycles
    cycles += 4;

    int carryValue = (carry && f.getC()) ? 1 : 0;
    u16 value = term1 + term2 + carryValue;

    f.setN(0);
    f.setH((term1 & 0xFFF) + (term2 & 0xFFF) + carryValue > 0xFFF);
    f.setC((term1 + term2 + carryValue) > 0xFFFF);

    return value;
}
u16 CPU::add(u16 term1, u8 term2) {  //4 extra cycles
    cycles += 4;

    u16 extendedTerm2 = extend(term2);
    u16 value = term1 + extendedTerm2;

    f.setZ(0);
    f.setN(0);
    f.setH((term1 & 0xF) + (term2 & 0xF) > 0xF);
    f.setC((term1 & 0xFF) + (term2 & 0xFF) > 0xFF);

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
    cycles += 4;

    int carryValue = (carry && f.getC()) ? 1 : 0;
    u16 value = term1 - term2 - carryValue;

    f.setN(1);
    f.setH((term1 & 0xFFF) - (term2 & 0xFFF) - carryValue < 0);
    f.setC((term1 - term2 - carryValue) < 0);

    return value;
}

u8 CPU::land(u8 term1, u8 term2) {
    u8 newValue = term1 & term2;

    f.setZ(newValue == 0);
    f.setN(0);
    f.setH(1);
    f.setC(0);

    return newValue;
}
u8 CPU::lxor(u8 term1, u8 term2) {
    u8 newValue = term1 ^ term2;

    f.setZ(newValue == 0);
    f.setN(0);
    f.setH(0);
    f.setC(0);

    return newValue;
}
u8 CPU::lor(u8 term1, u8 term2) {
    u8 newValue = term1 | term2;

    f.setZ(newValue == 0);
    f.setN(0);
    f.setH(0);
    f.setC(0);

    return newValue;
}
void CPU::cp(u8 term1, u8 term2) {
    sub(term1, term2);
}

void CPU::push(u16 value) {  //8 cycles, the actual opcodes have 4 extra cycles
    write16(SP - 2, value);
    SP -= 2;
}
void CPU::pushOP(u16 value) {  //Actual push operation, requires 4 extra cycles
    push(value);
    cycles += 4;
}
u16 CPU::pop() {  //8 cycles
    u16 value = read16(SP);
    SP += 2;
    return value;
}

void CPU::jr(bool cond) {
    u16 newPC = extend(fetch()) + PC;
    if (cond) setPC(newPC);
}
void CPU::jp(bool cond) {
    u16 newPC = fetch16();
    if (cond) setPC(newPC);
}
void CPU::call(bool cond) {
    u16 newPC = fetch16();
    if (cond) {
        push(PC);
        setPC(newPC);
    }
}
void CPU::ret() {
    setPC(pop());
}
void CPU::ret(bool cond) {
    cycles += 4;  //Internal 4 extra cycles
    if (cond) {
        setPC(pop());
    }
}
void CPU::rst(u16 value) {
    push(PC);
    setPC(value);
}

u16 CPU::extend(u8 value) {
    return (u16)(i16)(i8)value;
}

void CPU::printDebug() {
    printf("PC: %04x, OP: %02x, A: %02x, B: %02x, C: %02x, D: %02x, E: %02x, F: %02x, H: %02x, L: %02x, Z: %d, N: %d, H: %d, C: %d\n", PC, memory.read(PC), a.get(), b.get(), c.get(), d.get(), e.get(), f.get(), h.get(), l.get(), f.getZ(), f.getN(), f.getH(), f.getC());
}