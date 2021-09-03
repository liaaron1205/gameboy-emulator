#include "gpu.h"

void GPU::setCurrentScanline() { currentScanline = 0; }
void GPU::setLcdStatus(u8 value) { lcdStatus = value; }

u8 GPU::getCurrentScanline() { return currentScanline; }
u8 GPU::getLcdStatus() { return lcdStatus; }

bool GPU::isLcdEnabled() { return lcdStatus & (1 << 7); }

void GPU::tick() {
    if (isLcdEnabled())
        scanlineCounter++;
    else
        return;

    if (scanlineCounter == 456) {
        currentScanline++;
        scanlineCounter = 0;

        // if (currentScanline == 144) Request interrupt
        if (currentScanline > 153) currentScanline = 0;
        // if (currentScanline < 144) Draw scanline
    }
}
