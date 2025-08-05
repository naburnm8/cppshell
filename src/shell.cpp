//
// Created by Me on 04/08/2025.
//

#include "shell.h"

void Shell::loop() {
    std::string line;
    while (true) {
        std::cout << env.currentShellOwner << ">";
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
        currentCommand->execute(&env);
    }
}