#include <stdio.h>

#include <iostream>

#include "gameboy.h"

using namespace std;

int main() {
    GameBoy gb = GameBoy("tests/cpu_instrs/cpu_instrs.gb");
    // GameBoy gb = GameBoy("tests/cpu_instrs/individual/11-op a,(hl).gb");
    while (1) {
        gb.tick();
    }
}
