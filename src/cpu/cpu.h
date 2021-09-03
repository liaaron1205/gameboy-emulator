#pragma once
#include "../memory/memory_bus.h"
#include "interrupt_manager.h"
#include "registers.h"

class CPU {
   public:  //Temp
    MemoryBus& memory;
    InterruptManager& interruptManager;

    Register a, b, c, d, e, h, l;
    FlagRegister f;
    RegisterPair af = RegisterPair(a, f);
    RegisterPair bc = RegisterPair(b, c);
    RegisterPair de = RegisterPair(d, e);
    RegisterPair hl = RegisterPair(h, l);

    u16 SP;
    u16 PC;

    int cycles = 0;

    bool halted = 0;

   public:
    CPU(MemoryBus& memory, InterruptManager& interruptManager);
    ~CPU() = default;

    void tick();

   public:  //Temp
    void printDebug();

    void serviceInterrupts(u16 address);

    void fetchDecodeExecute();
    void fetchDecodeExecutePrefix();

    u8 read(u16 address);
    u16 read16(u16 address);
    void write(u16 address, u8 value);
    void write16(u16 address, u16 value);

    u8 fetch();
    u16 fetch16();

    void setPC(u16 value);

    u8 inc(u8 value);
    u16 inc(u16 value);
    u8 dec(u8 value);
    u16 dec(u16 value);

    u8 rlc(u8 value, bool checkZero = 1);
    u8 rl(u8 value, bool checkZero = 1);
    u8 rrc(u8 value, bool checkZero = 1);
    u8 rr(u8 value, bool checkZero = 1);
    u8 sla(u8 value);
    u8 sra(u8 value);
    u8 srl(u8 value);
    u8 swap(u8 value);

    void bit(int num, u8 value);
    u8 res(int num, u8 value);
    u8 set(int num, u8 value);

    u8 add(u8 term1, u8 term2, bool carry = 0);
    u16 add(u16 term1, u16 term2, bool carry = 0);
    u16 add(u16 term1, u8 term2);
    u8 sub(u8 term1, u8 term2, bool carry = 0);
    u16 sub(u16 term1, u16 term2, bool carry = 0);

    u8 land(u8 term1, u8 term2);
    u8 lxor(u8 term1, u8 term2);
    u8 lor(u8 term1, u8 term2);
    void cp(u8 term1, u8 term2);

    void push(u16 value);
    void pushOP(u16 value);
    u16 pop();

    void jr(bool cond = 1);
    void jp(bool cond = 1);
    void call(bool cond = 1);
    void ret();
    void ret(bool cond);
    void rst(u16 value);

    u16 extend(u8 value);
};
/*

Reading and writing from system memory takes 4 cycles

Writing to a 16 bit register also takes 4 cycles

Other operations take 0 cycles

Note: for 16bit operation, the carry from bit11 to 12 (3 to 4 in the upper byte) sets the half carry flag

*/