#pragma once
#include "../memory/memory_bus.h"
#include "registers.h"

class CPU {
   public:  //Temp
    MemoryBus memory;

    Register a, b, c, d, e, h, l;
    FlagRegister f;
    RegisterPair af = RegisterPair(a, f);
    RegisterPair bc = RegisterPair(b, c);
    RegisterPair de = RegisterPair(d, e);
    RegisterPair hl = RegisterPair(h, l);

    u16 SP;
    u16 PC;

    int cycles = 0;

   public:
    CPU();
    ~CPU() = default;

    void fetchDecodeExecute();
    int getCycles();

   public:  //Temp
    u8 read(u16 address);
    u16 read16(u16 address);
    void write(u16 address, u8 value);
    void write16(u16 address, u16 value);

    u8 fetch();
    u16 fetch16();

    void setPair(RegisterPair& reg, u16 value);
    void setSP(u16 value);
    void setPC(u16 value);

    u8 inc(u8 value);
    u16 inc(u16 value);
    u8 dec(u8 value);
    u16 dec(u16 value);

    u8 rlc(u8 value, bool checkZero = true);
    u8 rl(u8 value, bool checkZero = true);
    u8 rrc(u8 value, bool checkZero = true);
    u8 rr(u8 value, bool checkZero = true);

    u8 add(u8 term1, u8 term2, bool carry = false);
    u16 add(u16 term1, u16 term2, bool carry = false);
    u8 sub(u8 term1, u8 term2, bool carry = false);
    u16 sub(u16 term1, u16 term2, bool carry = false);
};

/*
Reading and writing from system memory takes 4 cycles

Writing to a 16 bit register also takes 4 cycles

Other operations take 0 cycles

Note: for 16bit operation, the carry from bit11 to 12 (3 to 4 in the upper byte) sets the half carry flag.

*/