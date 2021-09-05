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

        draw();

        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(std::floor(16.666f - elapsedMS));
    }
}

void GameBoy::draw() {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            GPU::Color color = gpu.frameBuffer[x][y];
            switch (color) {
                case GPU::Color::black:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    break;
                case GPU::Color::darkGray:
                    SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
                    break;
                case GPU::Color::lightGray:
                    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                    break;
                case GPU::Color::white:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    break;
            }
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);
}

void GameBoy::tick() {
    timer.tick();
    cpu.tick();
    memory.tick();
    gpu.tick();
}