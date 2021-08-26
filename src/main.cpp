#include <stdio.h>

#include <iostream>

#include "cpu/cpu.h"

using namespace std;

int main() {
    int x = 4;
    int y = x += 2;
    cout << y << endl;
}
