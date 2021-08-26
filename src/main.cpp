#include <stdio.h>

#include <iostream>

#include "cpu/cpu.h"

using namespace std;

int x = 3;

int y() {
    x++;
    return 3;
}
int main() {
    cout << x + y() << endl;
}
