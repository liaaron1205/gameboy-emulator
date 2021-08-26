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

    void set_z(bool value);
    void set_n(bool value);
    void set_h(bool value);
    void set_c(bool value);

    bool get_z();
    bool get_n();
    bool get_h();
    bool get_c();
};

class RegisterPair {
    Register &hi, &lo;

   public:
    RegisterPair(Register &hi, Register &lo);
    ~RegisterPair() = default;

    void set(u16 value);
    u16 get();

    void inc();
    void dec();
};

const int Z_FLAG = 7;
const int N_FLAG = 6;
const int H_FLAG = 5;
const int C_FLAG = 4;
