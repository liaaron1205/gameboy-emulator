#pragma once

#include "../constants.h"
#include "../cpu/interrupt_manager.h"

class GPU {
    enum Mode {
        HBlank,
        VBlank,
        OAM,
        VRAM
    } mode = HBlank;
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

    //0xFF42/0xFF43, X/Y position of the BACKGROUND to start drawing the viewing area
    u8 SCX, SCY;

    //0xFF44 Y-value of current line, 0xFF45 Used to compare with LY
    u8 LY = 0, LYC = 0;

    InterruptManager& interruptManager;

   public:
    GPU(InterruptManager& interruptManager);

    u8 getLcdControl();
    u8 getStat();
    u8 getSCX();
    u8 getSCY();
    u8 getLY();
    u8 getLYC();

    void setLcdControl(u8 value);
    void setStat(u8 value);
    void setSCX(u8 value);
    void setSCY(u8 value);
    void setLY();
    void setLYC(u8 value);

    void run();

   private:
    void tick();
};