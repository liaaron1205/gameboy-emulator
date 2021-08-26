#include "../constants.h"

class Register {
   protected:
    u8 val;

   public:
    Register();
    ~Register() = default;

    void set(u8 value);
    u8 get();
};

class FlagRegister : public Register {
   public:
    FlagRegister();
    ~FlagRegister() = default;

    void setZ(bool value);
    void setN(bool value);
    void setH(bool value);
    void setC(bool value);

    bool getZ();
    bool getN();
    bool getH();
    bool getC();
};

class RegisterPair {
    Register &hi, &lo;

   public:
    RegisterPair(Register &hi, Register &lo);
    ~RegisterPair() = default;

    void set(u16 value);
    u16 get();

    u16 pp();  //Like ++, increments and returns un-incremented value
    u16 mm();  //Like --, decrements and returns un-decremented value
};

const int Z_FLAG = 7;
const int N_FLAG = 6;
const int H_FLAG = 5;
const int C_FLAG = 4;
