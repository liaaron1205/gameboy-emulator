#include "../constants.h"

class GPU {
    enum Mode {
        HBlank,
        VBlank,
        OAM,
        VRAM
    } mode = HBlank;
    int modeTicks = 0;

    u8 LY = 0;  //Line Number,

   public:
    void tick();

   private:
};