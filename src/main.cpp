#include <stdio.h>

#include <iostream>

#include "cpu/cpu.h"

using namespace std;

int main() {
    u16 a = 0xFF00;
    u16 b = 0xFFFF;
    bool c = (a - b - 1) < 0;
    cout << (a - b + 1) << endl;
}
