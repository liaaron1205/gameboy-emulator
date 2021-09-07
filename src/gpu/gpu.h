#pragma once

#include "../constants.h"
#include "../cpu/interrupt_manager.h"

class GPU {
   public:
    enum Color {
        white,
        lightGray,
        darkGray,
        black,
    };
    std::vector<std::vector<Color>> frameBuffer;

   private:
    enum Mode {
        HBlank,
        VBlank,
        scanlineOAM,
        scanlineVRAM
    } mode = HBlank;

    std::vector<u8> OAM;
    std::vector<u8> tileData[3];
    std::vector<u8> tileMap[2];

    int modeTicks = 0;

    /*
    0xFF40, LCD Control
    7	Display: on/off
    6	Window Tile Map Display Select (0 = 9800-9BFF, 1 = 9C00-9FFF)
    5	Window Display: on/off  
    4	BG & Window Tile Data Select   (0 = 8800-97FF, 1 = 8000-8FFF)
    3	BG Tile Map Display Select     (0 = 9800-9BFF, 1 = 9C00-9FFF)
    2	OBJ (Sprite) Size (0=8x8, 1=8x16)
    1	OBJ (Sprite) Display Enable (0=Off, 1=On)
    0    
    */
    u8 lcdControl;
    /*
    0xFF41, Status
    6:  LYC = LY Coincidence Interrupt (1=Enable) (Read/Write)
    5:  Enable OAM interrupt
    4:  Enable VBlank interrupt
    3:  Enable HBlank interrupt
    2:  Coincidence Bit (0xFF44 == 0xFF45)
    */
    u8 stat;

    //0xFF42/0xFF43, Y/X position of the BACKGROUND to start drawing the viewing area
    u8 SCY, SCX;

    //0xFF44 Y-value of current line, 0xFF45 Used to compare with LY
    u8 LY = 0, LYC = 0;

    //0xFF47-0xFF49, Palette registers
    u8 bgPalette, obj0Palette, obj1Palette;

    //0xFF4A/0xFF4B, Y/X position of the VIEWING AREA to start drawing the window, note X position is -7
    u8 WY, WX;

    int enableDelay = 0;
    bool wasDisabled = 0;

    InterruptManager& interruptManager;

   public:
    GPU(InterruptManager& interruptManager);

    u8 readOAM(u16 address);
    u8 readVRAM(u16 address);
    void writeOAM(u16 address, u8 value);
    void writeVRAM(u16 address, u8 value);

    u8 getLcdControl();
    u8 getStat();
    u8 getSCX();
    u8 getSCY();
    u8 getLY();
    u8 getLYC();
    u8 getBgPalette();
    u8 getObj0Palette();
    u8 getObj1Palette();
    u8 getWY();
    u8 getWX();

    void setLcdControl(u8 value);
    void setStat(u8 value);
    void setSCX(u8 value);
    void setSCY(u8 value);
    void setLY();
    void setLYC(u8 value);
    void setBgPalette(u8 value);
    void setObj0Palette(u8 value);
    void setObj1Palette(u8 value);
    void setWY(u8 value);
    void setWX(u8 value);

    bool getLcdEnabled();
    bool getWindowTileMap();
    bool getWindowEnabled();
    bool getBgWindowTileData();
    bool getBgTileMapDisplay();
    bool getSpriteSize();
    bool getSpriteEnabled();
    bool getBgEnabled();

    void tick();

   private:
    void renderScanline();
    void renderTile(int x, int y, int realX, int realY, bool tileMapIndex, bool tileSelect, u8 palette);
    void renderSprites();

    Color getPalette(u8 value, int idx);
    int getColorIndex(u8 tileMsb, u8 tileLsb, int tileXpos);
};