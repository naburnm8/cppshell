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
        if (currentArgList != nullptr) {
            argumentHistory.push_back(currentArgList);
        }
        if (currentExecutionEffect != nullptr) {
            effectHistory.push_back(currentExecutionEffect);
        }

        currentArgList = mapArguments(line, &env);
        currentCommand = createCommand(currentArgList, &env);
        currentExecutionEffect = createEffect(currentArgList, &env);

        if (currentExecutionEffect != nullptr) {
            currentExecutionEffect->commenceEffect();
        }

        currentCommand->execute(&env);

        if (currentExecutionEffect != nullptr) {
            currentExecutionEffect->cleanup();
        }
    }
}
