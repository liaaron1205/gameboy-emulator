#include <stdio.h>

#include <iostream>

#include "gameboy.h"

using namespace std;

int main() {
    //GameBoy gb = GameBoy("tests/cpu_instrs/individual/02-interrupts.gb");
    GameBoy gb = GameBoy("tests/cpu_instrs/cpu_instrs.gb");
    while (1) {
        gb.tick();
    }
}
