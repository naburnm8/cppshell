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
    std::vector<ArgumentList*> argumentHistory;
    std::vector<ExecutionEffect*> effectHistory;
    Environment env;
    Command* currentCommand = nullptr;
    ArgumentList* currentArgList = nullptr;
    ExecutionEffect* currentExecutionEffect = nullptr;
public:
    Shell();
    void loop ();
};




#endif //SHELL_H
