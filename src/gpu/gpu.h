#include "../constants.h"

class GPU {
    u8 lcdStatus;        //0xFF41
    u8 currentScanline;  //0xFF44
    u8 scanlineCounter = 0;

   public:
    void tick();

    void setLcdStatus(u8 value);
    void setCurrentScanline();  //sets currentScanline to 0

    u8 getLcdStatus();
    u8 getCurrentScanline();

   private:
    bool isLcdEnabled();
};