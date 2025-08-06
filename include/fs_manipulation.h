//
// Created by Me on 06/08/2025.
//

#ifndef FS_MANIPULATION_H
#define FS_MANIPULATION_H
#include "command.h"

namespace Commands {
    class Rem final : public Command {
    public:
        explicit Rem(ArgumentList args): Command(std::move(args)) {}
        void execute(Environment* env) override;
        ~Rem() override = default;
    };

    class RemTree final : public Command {
        public:
        explicit RemTree(ArgumentList args): Command(std::move(args)) {}
        void execute(Environment* env) override;
        ~RemTree() override = default;
    };

    class MkDir final : public Command {
        public:
        explicit MkDir(ArgumentList args): Command(std::move(args)) {}
        void execute(Environment* env) override;
        ~MkDir() override = default;
    };

    class MkFile final : public Command {
        public:
        explicit MkFile(ArgumentList args): Command(std::move(args)) {}
        void execute(Environment* env) override;
        ~MkFile() override = default;
    };

    class Copy final : public Command {
        public:
        explicit Copy(ArgumentList args): Command(std::move(args)) {}
        void execute(Environment* env) override;
        ~Copy() override = default;
    };

    class Paste final : public Command {
        public:
        explicit Paste(ArgumentList args): Command(std::move(args)) {}
        void execute(Environment* env) override;
        ~Paste() override = default;
    };
}

inline std::unordered_map<std::string, CommandFactory> fsRegistry = {
    {"rem", [](const ArgumentList& args) -> Command* {
        return new Commands::Rem(args);
    }},
    {
        "remtree", [](const ArgumentList& args) -> Command* {
            return new Commands::RemTree(args);
        }
    },
    {
        "mkdir", [](const ArgumentList& args) -> Command* {
            return new Commands::MkDir(args);
        }
    },
    {"mkfile", [](const ArgumentList& args) -> Command* {
        return new Commands::MkFile(args);
    }},
    {
    "copy", [](const ArgumentList& args) -> Command* {
        return new Commands::Copy(args);
    }
    },
    {
        "paste", [](const ArgumentList& args) -> Command* {
            return new Commands::Paste(args);
        }
    }
};
#endif //FS_MANIPULATION_H
