//
// Created by Me on 03/08/2025.
//

#ifndef SHELL_H
#define SHELL_H
#include <string>
#include <vector>
#include <iostream>

#include "command.h"

class Shell {
    std::vector<Command*> history;
    Environment env;
    Command* currentCommand = nullptr;
    // TODO: add pwd
public:
    void loop ();
};




#endif //SHELL_H
