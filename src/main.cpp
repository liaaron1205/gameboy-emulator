#include "gameboy.h"

int main(int argv, char** args) {
    GameBoy gb = GameBoy("tests/gb-test-roms-master/instr_timing/instr_timing.gb");
    gb.run();
}