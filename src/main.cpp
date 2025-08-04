#include <iostream>
#include "shell.h"

int main() {
    auto* shell = new Shell();
    shell->loop();
    return 0;
}