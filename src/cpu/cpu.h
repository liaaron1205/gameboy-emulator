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

    u16 stackPointer;
    u16 PC;

    int cycles = 0;

   public:
    CPU();
    ~CPU() = default;

    void fetchDecodeExecute();
    int getCycles();

   private:
    u8 read(u16 address);
    u16 read16(u16 address);
    void write(u16 address, u8 value);
    void write16(u16 address, u16 value);

    u8 fetch();
    u16 fetch16();
};

/*
Reading and writing from system memory takes 4 cycles

Writing to a 16 bit register also takes 4 cycles

Other operations take 0 cycles

*/