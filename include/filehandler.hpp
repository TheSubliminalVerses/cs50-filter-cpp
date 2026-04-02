//
// Created by mpolesensky on 02/04/2026.
//

#ifndef FILTER_FILEHANDLER_HPP
#define FILTER_FILEHANDLER_HPP

#include <fstream>
#include <vector>

#include "bmp.hpp"

class FileHandler {
    std::ifstream src;
    std::ofstream dst;

    BMPFIleHeader bfh{};
    BMPInfoHeader bih{};

    size_t width{0};
    size_t height{0};

    std::vector<std::vector<Pixel>> image{};

public:
    explicit FileHandler(const std::string &path);
    ~FileHandler();

    const BMPFIleHeader &getFileHeader() const;
    const BMPInfoHeader &getInfoHeader() const;

    std::vector<std::vector<Pixel>> &getImage();

    void openOutput(const std::string &path);
    void writeHeaders();
    void writeImage();
};

#endif //FILTER_FILEHANDLER_HPP
