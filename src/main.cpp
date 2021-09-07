#include "gameboy.h"

int main(int argv, char** args) {
    //GameBoy gb = GameBoy("tests/mooneye/acceptance/bits/unused_hwio-GS.gb");
    // GameBoy gb = GameBoy("tests/mooneye/acceptance/oam_dma_restart.gb");
    //GameBoy gb = GameBoy("tests/gb-test-roms-master/mem_timing-2/rom_singles/01-read_timing.gb");
    //GameBoy gb = GameBoy("tests/gb-test-roms-master/mem_timing-2/mem_timing.gb");
    GameBoy gb = GameBoy("tests/games/Dr. Mario.gb");
    gb.run();
}