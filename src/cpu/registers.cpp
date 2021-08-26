#include "registers.h"

Register::Register() { val = 0; }

u8 Register::get() { return val; }

void Register::set(u8 value) { val = value; }

void FlagRegister::set_z(bool value) {
    val &= ~(1 << Z_FLAG);
    if (value) val |= (1 << Z_FLAG);
}
void FlagRegister::set_n(bool value) {
    val &= ~(1 << N_FLAG);
    if (value) val |= (1 << N_FLAG);
}
void FlagRegister::set_h(bool value) {
    val &= ~(1 << H_FLAG);
    if (value) val |= (1 << H_FLAG);
}
void FlagRegister::set_c(bool value) {
    val &= ~(1 << C_FLAG);
    if (value) val |= (1 << C_FLAG);
}

bool FlagRegister::get_z() {
    return val & (1 << Z_FLAG);
}
bool FlagRegister::get_n() {
    return val & (1 << N_FLAG);
}
bool FlagRegister::get_h() {
    return val & (1 << H_FLAG);
}
bool FlagRegister::get_c() {
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