//
// Created by Me on 25/09/2025.
//

#ifndef HEX_CAT_H
#define HEX_CAT_H

#include "command.h"

namespace Commands{
    class HexCat final : public Command {
    public:
        explicit HexCat(const ArgumentList &args)
            : Command(args) {
        }
        ~HexCat() override = default;
        void execute(Environment *env) override;
    };
}


inline std::unordered_map<std::string, CommandFactory> hexCatRegistry = {
    {"hexcat", [](const ArgumentList& args) -> Command* {
        return new Commands::HexCat(args);
    }}
};

#endif //HEX_CAT_H
