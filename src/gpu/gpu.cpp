#include "gpu.h"

GPU::GPU(InterruptManager& interruptManager) : interruptManager(interruptManager){};

u8 GPU::getLcdControl() { return lcdControl; }
u8 GPU::getStat() { return stat; }
u8 GPU::getSCX() { return SCX; }
u8 GPU::getSCY() { return SCY; }
u8 GPU::getLY() { return LY; }
u8 GPU::getLYC() { return LYC; }

void GPU::setLcdControl(u8 value) { lcdControl = value; }
void GPU::setStat(u8 value) { stat = (stat & 0x3) & (value & 0xF8); }  //Preserve the lower 3 bits
void GPU::setSCX(u8 value) { SCX = value; }
void GPU::setSCY(u8 value) { SCY = value; }
void GPU::setLY() { LY = 0; }  //TODO Only allow this when screen is disabled
void GPU::setLYC(u8 value) { LYC = value; }

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

                if (LY == LYC) {
                    if (stat & (1 << 6)) interruptManager.requestInterrupt(InterruptManager::LCD);
                }
                //TODO Write a scanline to the framebuffer
                if (stat & (1 << 3)) interruptManager.requestInterrupt(InterruptManager::LCD);
            }
            break;
        case HBlank:
            if (modeTicks == 204) {
                modeTicks = 0;
                LY++;

                if (LY == 144) {
                    mode = VBlank;
                    interruptManager.requestInterrupt(InterruptManager::Vblank);
                    if (stat & (1 << 4)) interruptManager.requestInterrupt(InterruptManager::LCD);
                } else
                    mode = OAM;
            }
            break;
        case VBlank:
            if (modeTicks == 4 && LY == 153 && (stat & (1 << 5))) interruptManager.requestInterrupt(InterruptManager::LCD);
            if (modeTicks == 456) {
                modeTicks = 0;
                LY++;
                if (LY == 154) {
                    mode = OAM;
                    LY = 0;

                    if (stat & (1 << 5)) interruptManager.requestInterrupt(InterruptManager::LCD);
                }
            }
            break;
    }

    stat = (stat & 0xF8) & (u8)(mode);
    if (LY == LYC)
        stat |= (1 << 2);
    else
        stat &= ~(1 << 2);
}