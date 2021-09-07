#include "gameboy.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "Please enter a rom" << std::endl;
    }
    std::string path = argv[1];
    GameBoy gb = GameBoy(path);
    gb.run();
}