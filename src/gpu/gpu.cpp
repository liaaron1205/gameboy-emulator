#include "gpu.h"

void GPU::tick() {
    modeTicks++;

    switch (mode) {
        case OAM:
            if (modeTicks == 80) {
                modeTicks = 0;
                mode = VRAM;
            }
            break;
        case VRAM:
            if (modeTicks == 172) {
                modeTicks = 0;
                mode = HBlank;

                //TODO Write a scanline to the framebuffer
            }
            break;
        case HBlank:
            if (modeTicks == 204) {
                modeTicks = 0;
                LY++;

                if (LY == 144) {
                    mode = VBlank;
                } else
                    mode = OAM;
            }
            break;
        case VBlank:
            if (modeTicks == 456) {
                modeTicks = 0;
                LY++;
                if (LY == 154) {
                    mode = OAM;
                    LY = 0;
                }
            }
            break;
    }
}