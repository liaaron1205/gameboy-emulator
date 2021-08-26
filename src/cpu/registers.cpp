#include "registers.h"

Register::Register() { val = 0; }

u8 Register::get() { return val; }

void Register::set(u8 value) { val = value; }

FlagRegister::FlagRegister() { val = 0; }

void FlagRegister::setZ(bool value) {
    val &= ~(1 << Z_FLAG);
    if (value) val |= (1 << Z_FLAG);
}
void FlagRegister::setN(bool value) {
    val &= ~(1 << N_FLAG);
    if (value) val |= (1 << N_FLAG);
}
void FlagRegister::setH(bool value) {
    val &= ~(1 << H_FLAG);
    if (value) val |= (1 << H_FLAG);
}
void FlagRegister::setC(bool value) {
    val &= ~(1 << C_FLAG);
    if (value) val |= (1 << C_FLAG);
}

bool FlagRegister::getZ() {
    return val & (1 << Z_FLAG);
}
bool FlagRegister::getN() {
    return val & (1 << N_FLAG);
}
bool FlagRegister::getH() {
    return val & (1 << H_FLAG);
}
bool FlagRegister::getC() {
    return val & (1 << C_FLAG);
}

RegisterPair::RegisterPair(Register &hi, Register &lo) : hi(hi), lo(lo) {}

u16 RegisterPair::get() {
    return (u16)lo.get() | ((u16)hi.get() << 8);
}

void RegisterPair::set(u16 value) {
    lo.set((u8)(value & 0x00FF));
    hi.set((u8)((value & 0xFF00) >> 8));
}