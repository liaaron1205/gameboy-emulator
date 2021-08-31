#include "cpu/cpu.h"
#include "cpu/interrupt_manager.h"
#include "memory/memory_bus.h"
#include "timer/timer.h"

class GameBoy {
   public:
    InterruptManager interruptManager;
    Timer timer = Timer(interruptManager);
    MemoryBus memory = MemoryBus(interruptManager, timer);
    CPU cpu = CPU(memory, interruptManager);

   public:
    GameBoy(std::string filename);
    ~GameBoy() = default;

    void tick();  //Run 60 times a second
};