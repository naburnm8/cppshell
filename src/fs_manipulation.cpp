//
// Created by Me on 06/08/2025.
//

#include <fstream>
#include "fs_manipulation.h"

bool Wildcard::isWildcard(const std::string &input) {
    std::string token;
    std::vector<std::string> tokens;
    for (int i = 0; i < input.length(); i++) {
        char c = input[i];
        if (c == '.') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
    }

    if (tokens.size() != 2 && tokens[0] != "*" && tokens[1] != "*") {
        return false;
    }
    this->name = tokens[0];
    this->extension = "." + tokens[1];
    return true;
}

std::vector<std::string> resolveWildcard(const Wildcard& wildcard, const std::filesystem::path& path) {
    std::vector<std::string> paths;
    try {
        for (const auto& entry: std::filesystem::directory_iterator(path.parent_path())) {
            if (entry.is_regular_file()) {
                if ((entry.path().extension().string() == wildcard.getExtension() || wildcard.getExtension() == "*") && (entry.path().stem().string() == wildcard.getName() || wildcard.getName() == "*")) {
                    paths.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return paths;
}



void Commands::Rem::execute(Environment *env) {
    if (this->arguments.list.size() < 2) {
        displayWrongArgumentsMessage();
        return;
    }

    std::vector<std::string> pathsToDelete;
    const std::filesystem::path relativePath = std::filesystem::path(this->arguments.list[1].rawInfo).relative_path();
    std::filesystem::path absolutePath = absolute(relativePath);
    Wildcard wildcard;
    if (wildcard.isWildcard(relativePath.string())) {
        pathsToDelete = resolveWildcard(wildcard, absolutePath);
    } else {
        pathsToDelete.push_back(this->arguments.list[1].rawInfo);
    }
    for (const auto& path : pathsToDelete) {
        try {
            if (std::filesystem::directory_entry(path).is_directory()) {
                std::cout << "Can't delete a directory. Use 'remtree' instead" << std::endl;
                return;
            }
            if (!std::filesystem::remove(path)) {
                throw std::filesystem::filesystem_error("File was not deleted", std::error_code(-1, std::system_category()));
            }
            std::cout << "File " << std::filesystem::path(path).filename().string() << " was successfully deleted" << std::endl;
        } catch (std::filesystem::filesystem_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void Commands::RemTree::execute(Environment *env) {
    if (this->arguments.list.size() < 2) {
        displayWrongArgumentsMessage();
        return;
    }

    try {
        if (!std::filesystem::directory_entry(this->arguments.list[1].rawInfo).is_directory()) {
            std::cout << "Can't delete a file. Use 'rem' instead" << std::endl;
            return;
        }
        if (!std::filesystem::remove(arguments.list[1].rawInfo)) {
            throw std::filesystem::filesystem_error("Directory was not deleted", std::error_code(-1, std::system_category()));
        }
        std::cout << "Directory " << std::filesystem::path(arguments.list[1].rawInfo).filename().string() << " was successfully deleted" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Commands::MkDir::execute(Environment *env) {
    if (this->arguments.list.size() < 2) {
        displayWrongArgumentsMessage();
        return;
    }

    try {
        std::filesystem::create_directory(this->arguments.list[1].rawInfo);
    } catch (std::filesystem::filesystem_error &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Commands::MkFile::execute(Environment *env) {
    if (this->arguments.list.size() < 2) {
        displayWrongArgumentsMessage();
        return;
    }

    try {
        std::ofstream stream(this->arguments.list[1].rawInfo);
        if (stream.is_open()) {
            stream << "";
            stream.close();
        } else {
            throw std::filesystem::filesystem_error("File was not open", std::error_code(-1, std::system_category()));
        }
    } catch (std::filesystem::filesystem_error &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Commands::Copy::execute(Environment *env) {

}

void Commands::Paste::execute(Environment *env) {

}


