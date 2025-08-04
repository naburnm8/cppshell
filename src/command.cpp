//
// Created by Me on 03/08/2025.
//
#include "command.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <utility>

Error::Error(ArgumentList arguments) {
    this->message = "Generic error";
    this->arguments = std::move(arguments);
}

void Error::execute() {
    std::cout << this->message << " at: " << this->arguments.rawInfo << std::endl;
}


std::vector<std::string> splitBySpaces(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;

}

Command * mapCommand(const std::string& input) {
    ArgumentList args;
    args.rawInfo = input;
    const auto tokens = splitBySpaces(args.rawInfo);

    if (tokens.empty()) {
        return new Errors::CommandNotFound(args);
    }

    const std::string& commandName = tokens[0];
    args.list.push_back({commandName, ArgumentType::CommandName});

    for (int i = 1; i < tokens.size(); i++) {
        args.list.push_back({tokens[i], ArgumentType::Undefined});
    }

    auto it = commandMap.find(commandName);
    if (it != commandMap.end()) {
        return it->second(args);
    }
    return new Errors::CommandNotFound(args);
}

std::string Commands::Echo::prepareString() {
    std::string str;
    for (auto &[rawInfo, type] : this->arguments.list) {
        if (type == ArgumentType::CommandName) {
            continue;
        }
        str += rawInfo + " ";
    }
    return str;
}

void Commands::Echo::execute() {
    std::cout << prepareString() << std::endl;
}

void Commands::Exit::execute() {
    exit(0);
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

void Errors::CommandNotFound::execute() {
    if (this->arguments.list.empty()) {
        return;
    }
    if (this->arguments.list[0].type == ArgumentType::CommandName) {
        Error::execute();
    } else {
        throw ParsingException(this->arguments.rawInfo);
    }
}

