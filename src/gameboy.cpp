#include "gameboy.h"

GameBoy::GameBoy(std::string filename) {
    memory.loadCartridge(filename);
}

void GameBoy::tick() {
}
