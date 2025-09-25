//
// Created by Me on 25/09/2025.
//
#include "hex_cat.h"

#include <fstream>

constexpr int DEFAULT_COMMANDLINE_COLS = 16;

std::pmr::unordered_map<char, std::string> hexMap = {
    {0, "0"}, {1, "1"}, {2, "2"}, {3, "3"},
    {4, "4"}, {5, "5"}, {6, "6"}, {7, "7"},
    {8, "8"}, {9, "9"}, {10, "A"}, {11, "B"},
    {12, "C"}, {13, "D"}, {14, "E"}, {15, "F"}
};

std::string byteToHex(const unsigned char byte) {
    char low = byte & 0x0F;
    char high = byte >> 4;
    return hexMap.find(high)->second + hexMap.find(low)->second;
}

std::string byteToHexBigEndian(const unsigned char byte) {
    char low = byte & 0x0F;
    char high = byte >> 4;
    return hexMap.find(low)->second + hexMap.find(high)->second;
}

std::string shortToHex(const unsigned short num) {
    char low = num & 0xFF;
    char high = num >> 8;

    return byteToHex(high) + byteToHex(low);
}

std::string intToHex(const unsigned int num) {
    short low = num & 0xFFFF;
    short high = num >> 16;
    return shortToHex(high) + shortToHex(low);
}

void Commands::HexCat::execute(Environment *env) {
    if (this->arguments.list.size() < 2) {
        displayWrongArgumentsMessage();
    }
    try {
        if (std::ifstream file(this->arguments.list[1].rawInfo, std::ios::binary); file.is_open()) {
            unsigned int currentAddress = 0;
            std::string out;
            while (!file.eof()) {
                std::string line;
                line += intToHex(currentAddress) + " ";
                for (int i = 0; i < DEFAULT_COMMANDLINE_COLS; i++) {
                    char buf;
                    file.read(&buf, sizeof(char));
                    line += byteToHexBigEndian(buf) + " ";
                    currentAddress++;
                }
                out += line + "\n";
            }
            file.close();
            std::cout << out << std::endl;
        } else {
            throw std::filesystem::filesystem_error("File is not open", std::error_code(-1, std::system_category()));
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }
}


