#include <SDL2/SDL.h>

#include "cpu/cpu.h"
#include "cpu/interrupt_manager.h"
#include "gpu/gpu.h"
#include "memory/memory_bus.h"
#include "timer/timer.h"

class GameBoy {
   public:
    InterruptManager interruptManager;
    GPU gpu = GPU(interruptManager);
    Timer timer = Timer(interruptManager);
    MemoryBus memory = MemoryBus(interruptManager, timer, gpu);
    CPU cpu = CPU(memory, interruptManager);

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

   public:
    GameBoy(std::string filename);
    ~GameBoy() = default;

    void draw();
    void run();
    void tick();  //Run 60 times a second
   private:
};