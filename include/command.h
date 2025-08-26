//
// Created by Me on 03/08/2025.
//

#ifndef COMMAND_H
#define COMMAND_H
#include <filesystem>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>



enum ArgumentType{
  Literal,
  PassToExecutable,
  ShellArgument,
  Ignored,
  RedirectSymbol,
  CommandName,
  Undefined,
};

struct Argument{
  std::string rawInfo;
  ArgumentType type;
};

struct ArgumentList {
  std::vector<Argument> list;
  std::string rawInfo;
};

class Command;

using CommandFactory = std::function<Command*(const ArgumentList&)>;

struct Environment {
  std::string currentShellOwner = "$";
  std::filesystem::path currentPath = std::filesystem::current_path();
  std::vector<std::unordered_map<std::string, CommandFactory>*> additionalCommandRegistries;

};
class Command {
  protected:
    ArgumentList arguments;
    void displayWrongArgumentsMessage() {
      std::cout << "Wrong arguments configuration" << std::endl;
    }
  public:
    explicit Command(ArgumentList args) {
      arguments = std::move(args);
    };
    virtual ~Command() = default;

    virtual void execute(Environment* env) = 0;
};



class Error: public Command {
   protected:
     std::string message;
   public:
     explicit Error(ArgumentList arguments);
     void execute(Environment* env) override;
     ~Error() override = default;
};

namespace Errors {
  class CommandNotFound final : public Error {
  public:
    explicit CommandNotFound(ArgumentList arguments);
    void execute(Environment* env) override;
  };

}



namespace Commands {
  class Echo final : public Command {
    public:
      explicit Echo(ArgumentList args): Command(std::move(args)) {}
      std::string prepareString();
      void execute(Environment* env) override;
      ~Echo() override = default;
  };

  class Exit final : public Command {
    public:
    explicit Exit(ArgumentList args): Command(std::move(args)) {}
    void execute(Environment* env) override;
    ~Exit() override = default;
  };

  class Pwd final : public Command {
    public:
    explicit Pwd(ArgumentList args): Command(std::move(args)) {}
    void execute(Environment* env) override;
    ~Pwd() override = default;
  };

  class Cd final : public Command {
    public:
    explicit Cd(ArgumentList args): Command(std::move(args)) {}
    void execute(Environment* env) override;
    ~Cd() override = default;
  };
  class Cat final : public Command {
    public:
    explicit Cat(ArgumentList args): Command(std::move(args)) {}
    void execute(Environment* env) override;
    ~Cat() override = default;
  };

  class Dir final : public Command {
    public:
    explicit Dir(ArgumentList args): Command(std::move(args)) {}
    void execute(Environment* env) override;
    ~Dir() override = default;
  };
}



inline std::unordered_map<std::string, CommandFactory> commandMap = {
  {
    "echo", [](const ArgumentList& arguments) -> Command* {
      return new Commands::Echo(arguments);
    }
  },
  { "exit", [](const ArgumentList& arguments) -> Command* {
    return new Commands::Exit(arguments);
  }
  },
  {"cat", [](const ArgumentList& arguments) -> Command* {
    return new Commands::Cat(arguments);
  }},
  {"pwd", [](const ArgumentList& arguments) -> Command* {
    return new Commands::Pwd(arguments);
  }},
  {"cd", [](const ArgumentList& arguments) -> Command* {
    return new Commands::Cd(arguments);
  }},
  {"ls", [](const ArgumentList& arguments) -> Command* {
    return new Commands::Dir(arguments);
  }},
  {"dir", [](const ArgumentList& arguments) -> Command* {
    return new Commands::Dir(arguments);
  }}
};

Command* mapCommand(const std::string& input, Environment* env);

struct ParsingException final : public std::exception {
  std::string input;
  explicit ParsingException(const std::string &input);
  [[nodiscard]] const char* what() const noexcept override;
};



#endif
