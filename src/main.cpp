#include <stdio.h>

#include <iostream>

#include "cpu/cpu.h"
#include "memory/memory_bus.h"

using namespace std;

int x = 3;

int y() {
    x++;
    return 3;
}
int main() {
    CPU cpu("asdf");
    cpu.test();
}
