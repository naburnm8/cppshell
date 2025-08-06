//
// Created by Me on 04/08/2025.
//

#include "shell.h"
#include "fs_manipulation.h"

Shell::Shell() {
    env.additionalCommandRegistries.push_back(&fsRegistry);
}

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
        currentCommand = mapCommand(line, &env);
        currentCommand->execute(&env);
    }
}
