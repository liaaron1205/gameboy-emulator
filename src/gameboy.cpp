#include "gameboy.h"

GameBoy::GameBoy(std::string filename) {
    memory.loadCartridge(filename);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
}

void GameBoy::run() {
    bool isRunning = 1;
    while (isRunning) {
        Uint64 start = SDL_GetPerformanceCounter();
        int cycles = 0;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = 0;
                    break;

                default:
                    break;
            }
        }

        while (cycles < CYCLES_PER_FRAME) {
            cycles++;
            tick();
        }

                Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(std::floor(16.666f - elapsedMS));
    }
}

void GameBoy::tick() {
    timer.tick();
    cpu.tick();
}