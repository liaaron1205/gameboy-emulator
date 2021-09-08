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
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case (SDLK_RIGHT):
                            joypad.keysPressed[Joypad::Keys::Right] = 1;
                            break;
                        case (SDLK_LEFT):
                            joypad.keysPressed[Joypad::Keys::Left] = 1;
                            break;
                        case (SDLK_UP):
                            joypad.keysPressed[Joypad::Keys::Up] = 1;
                            break;
                        case (SDLK_DOWN):
                            joypad.keysPressed[Joypad::Keys::Down] = 1;
                            break;
                        case (SDLK_x):
                            joypad.keysPressed[Joypad::Keys::A] = 1;
                            break;
                        case (SDLK_z):
                            joypad.keysPressed[Joypad::Keys::B] = 1;
                            break;
                        case (SDLK_SPACE):
                            joypad.keysPressed[Joypad::Keys::Select] = 1;
                            break;
                        case (SDLK_RETURN):
                            joypad.keysPressed[Joypad::Keys::Start] = 1;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case (SDLK_RIGHT):
                            joypad.keysPressed[Joypad::Keys::Right] = 0;
                            break;
                        case (SDLK_LEFT):
                            joypad.keysPressed[Joypad::Keys::Left] = 0;
                            break;
                        case (SDLK_UP):
                            joypad.keysPressed[Joypad::Keys::Up] = 0;
                            break;
                        case (SDLK_DOWN):
                            joypad.keysPressed[Joypad::Keys::Down] = 0;
                            break;
                        case (SDLK_x):
                            joypad.keysPressed[Joypad::Keys::A] = 0;
                            break;
                        case (SDLK_z):
                            joypad.keysPressed[Joypad::Keys::B] = 0;
                            break;
                        case (SDLK_SPACE):
                            joypad.keysPressed[Joypad::Keys::Select] = 0;
                            break;
                        case (SDLK_RETURN):
                            joypad.keysPressed[Joypad::Keys::Start] = 0;
                            break;
                    }
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
        int delay = std::floor(16.666f - elapsedMS);
        if (delay > 0)
            SDL_Delay(delay);
        // else
        // std::cerr << delay << std::endl;
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