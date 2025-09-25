//
// Created by Me on 03/08/2025.
//
#include "command.h"
#include "execution_effect.h"

#include <functional>
#include <iostream>
#include <utility>
#include <fstream>

CommandExecutable::CommandExecutable(const std::string &line) {

}

void CommandExecutable::execute() {
    for (const auto &effect: executionEffects) {
        effect->commenceEffect();
    }

    command->execute(environment);

    for (const auto &effect: executionEffects) {
        effect->cleanup();
    }
}

Error::Error(ArgumentList arguments): Command(std::move(arguments)) {
    this->message = "Generic error";
}

void Error::execute(Environment* env) {
    std::cout << this->message << " at: " << this->arguments.rawInfo << std::endl;
}


std::vector<std::string> splitBySpacesRespectQuotes(const std::string& input) {
    std::vector<std::string> tokens;
    std::string token;

    bool inQuotes = false;
    bool escapeSuspicion = false;
    char quoteChar = '\0';

    for (int i = 0; i < input.length(); i++) {
        const char c = input[i];
        if (inQuotes) {
            if (escapeSuspicion) {
                if (c == '"' || c == '\'') {
                    token += c;
                } else {
                    token += '\\';
                    token += c;
                }
                escapeSuspicion = false;
                continue;
            }
            if (c == quoteChar) {
                inQuotes = false;
                tokens.push_back(token);
                token.clear();
            } else if (c == '\\') {
                escapeSuspicion = true;
            }
            else {
                token += c;
            }
        } else if (escapeSuspicion) {
            if (c == '"' || c == '\'') {
                token += c;
            } else {
                token += '\\';
                token += c;
            }
            escapeSuspicion = false;
        }
        else {
            if (std::isspace(c)) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } else if (c == '"' || c == '\'') {
                inQuotes = true;
                quoteChar = c;
            } else if (c == '\\') {
                escapeSuspicion = true;
            } else {
                token += c;
            }
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;

}

ArgumentList parseArguments(const std::vector<std::string> &tokens, Environment* env) {
    ArgumentList args;

    const std::string& commandName = tokens[0];
    args.list.push_back({commandName, ArgumentType::CommandName});

    bool reachedEffectArguments = false;
    for (int i = 1; i < tokens.size(); i++) {
        if (!reachedEffectArguments) {
            auto it = effectsRegistry.find(tokens[i]);
            if (it == effectsRegistry.end()) {
                for (const auto registry: env->additionalEffectRegistries) {
                    it = registry->find(tokens[i]);
                    if (it != registry->end()) {
                        break;
                    }
                }
            }
            if (it != effectsRegistry.end()) {
                reachedEffectArguments = true;
                args.list.push_back({tokens[i], ArgumentType::EffectSymbol});
            } else {
                args.list.push_back({tokens[i], ArgumentType::GenericArgument});
            }
        } else {
            args.list.push_back({tokens[i], ArgumentType::EffectGenericArgument});
        }
    }

    return args;
}

ArgumentList* mapArguments(const std::string& input, Environment* env) {
    const auto tokens = splitBySpacesRespectQuotes(input);
    ArgumentList args = parseArguments(tokens, env);
    args.rawInfo = input;
    return new ArgumentList(args);
}

Command* createCommand(const ArgumentList* args, Environment* env) {
    if (args->list.empty()) {
        return new Errors::CommandNotFound(*args);
    }
    auto commandName = args->list[0].rawInfo;
    auto it = commandMap.find(commandName);
    if (it != commandMap.end()) {
        return it->second(*args);
    }
    if (env != nullptr) {
        for (const auto registry: env->additionalCommandRegistries) {
            auto itAdditional = registry->find(commandName);
            if (itAdditional != registry->end()) {
                return itAdditional->second(*args);
            }
        }
    }
    return new Errors::CommandNotFound(*args);
}


ExecutionEffect* createEffect(ArgumentList* args, Environment* env) {
    std::string effectName;
    for (const auto&[rawInfo, type]: args->list) {
        if (type == ArgumentType::EffectSymbol) {
            effectName = rawInfo;
            break;
        }
    }
    if (effectName.empty()) {
        return nullptr;
    }
    auto it = effectsRegistry.find(effectName);
    if (it != effectsRegistry.end()) {
        return it->second(*args);
    }
    if (env != nullptr) {
        for (const auto registry: env->additionalEffectRegistries) {
            auto it = registry->find(effectName);
            if (it != registry->end()) {
                return it->second(*args);
            }
        }
    }
    return nullptr;
}


Command * mapCommand(const std::string& input, Environment* env = nullptr) {
    ArgumentList args;
    args.rawInfo = input;
    const auto tokens = splitBySpacesRespectQuotes(args.rawInfo);

    if (tokens.empty()) {
        return new Errors::CommandNotFound(args);
    }

    const std::string& commandName = tokens[0];
    args.list.push_back({commandName, ArgumentType::CommandName});

    for (int i = 1; i < tokens.size(); i++) {
        args.list.push_back({tokens[i], ArgumentType::GenericArgument});
    }

    auto it = commandMap.find(commandName);
    if (it != commandMap.end()) {
        return it->second(args);
    }
    if (env != nullptr) {
        for (const auto registry: env->additionalCommandRegistries) {
            auto itInside = registry->find(commandName);
            if (itInside != registry->end()) {
                return itInside->second(args);
            }
        }
    }

    return new Errors::CommandNotFound(args);
}

std::string Commands::Echo::prepareString() {
    std::string str;
    for (auto &[rawInfo, type] : this->arguments.list) {
        if (type != ArgumentType::GenericArgument) {
            continue;
        }
        str += rawInfo + " ";
    }
    return str;
}

void Commands::Echo::execute(Environment* env) {
    std::cout << prepareString() << std::endl;
}

void Commands::Exit::execute(Environment* env) {
    exit(0);
}

void Commands::Pwd::execute(Environment* env) {
    std::cout << env->currentPath << std::endl;
}

void Commands::Cd::execute(Environment *env) {
    if (this->arguments.list.size() < 2) {
        return;
    }
    try {
        std::filesystem::current_path(this->arguments.list[1].rawInfo);
        env->currentPath = std::filesystem::current_path();
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Commands::Cat::execute(Environment* env) {
    if (this->arguments.list.size() < 2) {
        displayWrongArgumentsMessage();
        return;
    }
    try {
        if (std::ifstream file(this->arguments.list[1].rawInfo); file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << std::endl;
            }
            file.close();
        } else {
            throw std::filesystem::filesystem_error("File is not open", std::error_code(-1, std::system_category()));
        }

    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }

}

void displayContents(const std::filesystem::path& path) {
    try {
        std::cout << "Contents of: " << path << std::endl;
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                std::cout << "  FILE " << entry.path().filename() << " " << entry.file_size() << " B" << std::endl;
            } else if (entry.is_directory()) {
                std::cout << "FOLDER " << entry.path().filename() << std::endl;
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Commands::Dir::execute(Environment *env) {
    if (this->arguments.list.size() < 2) {
        displayContents(env->currentPath);
    }
    else {
        for (int i = 1; i < this->arguments.list.size(); i++) {
            displayContents(this->arguments.list[i].rawInfo);
        }
    }
}

ParsingException::ParsingException(const std::string &input) {
    this->input = input;
}

const char * ParsingException::what() const noexcept {
    return ("Parsing error at" + this->input).c_str();
}

Errors::CommandNotFound::CommandNotFound(ArgumentList arguments): Error(std::move(arguments)) {
    this->message = "Command not found";
}

void Errors::CommandNotFound::execute(Environment* env) {
    if (this->arguments.list.empty()) {
        return;
    }
    if (this->arguments.list[0].type == ArgumentType::CommandName) {
        Error::execute(nullptr);
    } else {
        throw ParsingException(this->arguments.rawInfo);
    }
}

