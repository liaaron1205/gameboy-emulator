#include "gameboy.h"

int main(int argv, char** args) {
    //GameBoy gb = GameBoy("tests/gb-test-roms-master/cpu_instrs/cpu_instrs.gb");
    GameBoy gb = GameBoy("tests/games/tetris.gb");
    gb.run();
}