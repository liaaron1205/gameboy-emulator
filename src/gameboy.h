#include "cpu/cpu.h"
#include "memory/memory_bus.h"

class GameBoy {
   public:
    MemoryBus memory;
    CPU cpu = CPU(memory);

   public:
    GameBoy(std::string filename);
    ~GameBoy() = default;

    void tick();  //Run 60 times a second
};