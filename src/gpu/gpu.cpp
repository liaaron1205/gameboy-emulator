#include "gpu.h"

GPU::GPU(InterruptManager& interruptManager) : OAM(0xA0),
                                               tileData{std::vector<u8>(0x800), std::vector<u8>(0x800), std::vector<u8>(0x800)},
                                               tileMap{std::vector<u8>(0x400), std::vector<u8>(0x400)},
                                               frameBuffer(WINDOW_WIDTH, std::vector<Color>(WINDOW_HEIGHT, Color::black)),
                                               interruptManager(interruptManager){};

u8 GPU::readOAM(u16 address) { return OAM[address - 0xFE00]; }
void GPU::writeOAM(u16 address, u8 value) { OAM[address - 0xFE00] = value; }
u8 GPU::readVRAM(u16 address) {
    if (address < 0x8800)
        return tileData[0][address - 0x8000];
    else if (address < 0x9000)
        return tileData[1][address - 0x8800];
    else if (address < 0x9800)
        return tileData[2][address - 0x9000];
    else if (address < 0x9C00)
        return tileMap[0][address - 0x9800];
    else if (address < 0xA000)
        return tileMap[1][address - 0x9C00];
    else
        return 0xFF;
}
void GPU::writeVRAM(u16 address, u8 value) {
    if (address < 0x8800)
        tileData[0][address - 0x8000] = value;
    else if (address < 0x9000)
        tileData[1][address - 0x8800] = value;
    else if (address < 0x9800)
        tileData[2][address - 0x9000] = value;
    else if (address < 0x9C00)
        tileMap[0][address - 0x9800] = value;
    else if (address < 0xA000)
        tileMap[1][address - 0x9C00] = value;
}

u8 GPU::getLcdControl() { return lcdControl; }
u8 GPU::getStat() { return stat; }
u8 GPU::getSCY() { return SCY; }
u8 GPU::getSCX() { return SCX; }
u8 GPU::getLY() { return LY; }
u8 GPU::getLYC() { return LYC; }
u8 GPU::getBgPalette() { return bgPalette; }
u8 GPU::getObj0Palette() { return obj0Palette; }
u8 GPU::getObj1Palette() { return obj1Palette; }
u8 GPU::getWY() { return WY; }
u8 GPU::getWX() { return WX; }

void GPU::setLcdControl(u8 value) { lcdControl = value; }
void GPU::setStat(u8 value) { stat = (stat & 0x3) & (value & 0xF8); }  //Preserve the lower 3 bits
void GPU::setSCY(u8 value) { SCY = value; }
void GPU::setSCX(u8 value) { SCX = value; }
void GPU::setLY() {
    if (getLcdEnabled()) LY = 0;
}
void GPU::setLYC(u8 value) { LYC = value; }
void GPU::setBgPalette(u8 value) { bgPalette = value; }
void GPU::setObj0Palette(u8 value) { obj0Palette = value; }
void GPU::setObj1Palette(u8 value) { obj1Palette = value; }
void GPU::setWY(u8 value) { WY = value; }
void GPU::setWX(u8 value) { WX = value; }

bool GPU::getLcdEnabled() { return (lcdControl & (1 << 7)); }
bool GPU::getWindowTileMap() { return (lcdControl & (1 << 6)); }
bool GPU::getWindowEnabled() { return (lcdControl & (1 << 5)); }
bool GPU::getBgWindowTileData() { return (lcdControl & (1 << 4)); }
bool GPU::getBgTileMapDisplay() { return (lcdControl & (1 << 3)); }
bool GPU::getSpriteSize() { return (lcdControl & (1 << 2)); }
bool GPU::getSpriteEnabled() { return (lcdControl & (1 << 1)); }
bool GPU::getBgEnabled() { return (lcdControl & (1 << 0)); }

void GPU::tick() {
    modeTicks++;

    switch (mode) {
        case scanlineOAM:
            if (modeTicks == 80) {
                modeTicks = 0;
                mode = scanlineVRAM;
            }
            break;
        case scanlineVRAM:
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
                    mode = scanlineOAM;
            }
            break;
        case VBlank:
            if (modeTicks == 4 && LY == 153 && (stat & (1 << 5))) interruptManager.requestInterrupt(InterruptManager::LCD);
            if (modeTicks == 456) {
                modeTicks = 0;
                LY++;
                if (LY == 154) {
                    mode = scanlineOAM;
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

void GPU::renderScanline() {
    int y = LY;

    for (int x = 0; x < 160; x++) {
        if (getBgEnabled()) {
            int realX = (x + SCX) % 256, realY = (y + SCY) % 256;
            renderTile(x, y, realX, realY, getBgTileMapDisplay(), getBgWindowTileData(), bgPalette);
        }
        if (getWindowEnabled() && y >= WY && x >= (WX - 7)) {
            int realX = x - (WX - 7), realY = (y - WY);
            renderTile(x, y, realX, realY, getWindowTileMap(), getBgWindowTileData(), bgPalette);
        }
    }
}

void GPU::renderTile(int x, int y, int realX, int realY, bool tileMapIndex, bool tileSelect, u8 palette) {
    int tileCol = realX / 8;
    int tileRow = realY / 8;

    int tileId = (tileRow * 32) + (tileCol);

    u8 tileNum = tileMap[tileMapIndex][tileId];

    int tileDataIndex = 2;
    if (tileNum >= 128) {
        tileNum %= 128;
        tileDataIndex = 1;
    } else if (tileSelect)
        tileDataIndex = 0;

    u16 tileAddress = tileNum * 16;

    int tileXpos = realX % 8;
    int tileYpos = realY & 8;

    u8 tileLsb = tileData[tileNum][tileAddress + tileYpos * 2];
    u8 tileMsb = tileData[tileNum][tileAddress + tileYpos * 2 + 1];

    Color color = getPalette(palette)[getColorIndex(tileMsb, tileLsb, tileXpos)];
    frameBuffer[x][y] = color;
}

std::vector<GPU::Color> GPU::getPalette(u8 value) {
    std::vector<GPU::Color> ret;
    ret.push_back((GPU::Color)(value & 0x03));
    ret.push_back((GPU::Color)((value & 0x0C) >> 2));
    ret.push_back((GPU::Color)((value & 0x30) >> 4));
    ret.push_back((GPU::Color)((value & 0xC0) >> 6));
    return ret;
}

int GPU::getColorIndex(u8 msb, u8 lsb, int bitPos) {
    u8 bitLsb = (u8)((u8)(lsb << bitPos) >> 7);
    u8 bitMsb = (u8)((u8)(msb << bitPos) >> 7);
    return (bitMsb << 1) | bitLsb;
}