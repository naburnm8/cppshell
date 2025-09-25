//
// Created by Me on 12/09/2025.
//

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include<string>
#include<vector>

enum ArgumentType{
    CommandName,
    GenericArgument,
    EffectSymbol,
    EffectGenericArgument
  };

struct Argument{
    std::string rawInfo;
    ArgumentType type;
};

struct ArgumentList {
    std::vector<Argument> list;
    std::string rawInfo;
    ArgumentList(const ArgumentList& other) {
        list = other.list;
        rawInfo = other.rawInfo;
    }
    ArgumentList() = default;
};



#endif //ARGUMENTS_H
