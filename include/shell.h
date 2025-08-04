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
    std::string currentShellOwner = "$> ";
    Command* currentCommand = nullptr;
    // TODO: add pwd
public:
    void loop () {
        std::string line;
        while (true) {
            std::cout << currentShellOwner;
            if (!std::getline(std::cin, line)) {
                break;
            }
            if (line.empty()) {
                continue;
            }
            if (currentCommand != nullptr) {
                history.push_back(currentCommand);
            }
            currentCommand = mapCommand(line);
            currentCommand->execute();
        }
    }

};


#endif //SHELL_H
