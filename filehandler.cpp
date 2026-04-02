//
// Created by mpolesensky on 02/04/2026.
//

#include "include/bmp.hpp"
#include "include/filehandler.hpp"

FileHandler::FileHandler(const std::string &path) : src(path, std::ios::in | std::ios::binary) {
    src.read(reinterpret_cast<char *>(&this->bfh), sizeof(BMPFIleHeader));
    src.read(reinterpret_cast<char *>(&this->bih), sizeof(BMPInfoHeader));

    const size_t abs_width = std::abs(this->bih.biWidth);
    const size_t abs_height = std::abs(this->bih.biHeight);

    this->width = abs_width;
    this->height = abs_height;

    for (int i = 0; i < abs_height; i++) {
        std::vector<Pixel> pixels{};
        for (int j = 0; j < abs_width; j++) {
            Pixel pixel{};
            this->src.read(reinterpret_cast<char *>(&pixel), sizeof(Pixel));
            pixels.push_back(pixel);
        }
        this->image.push_back(pixels);
    }
}

const BMPFIleHeader &FileHandler::getFileHeader() const {
    return this->bfh;
}

const BMPInfoHeader &FileHandler::getInfoHeader() const {
    return this->bih;
}

std::vector<std::vector<Pixel>> &FileHandler::getImage() {
    return this->image;
}

void FileHandler::openOutput(const std::string &path) {
    this->dst.open(path, std::ios::out | std::ios::binary);
}

void FileHandler::writeHeaders() {
    this->dst.write(reinterpret_cast<char *>(&this->bfh), sizeof(BMPFIleHeader));
    this->dst.write(reinterpret_cast<char *>(&this->bih), sizeof(BMPInfoHeader));
}

void FileHandler::writeImage() {
    // TODO: Add size checking later
    const long size = static_cast<long>(this->width * sizeof(Pixel));

    for (auto &row : this->image) {
        this->dst.write(reinterpret_cast<char *>(row.data()), size);
    }
}

FileHandler::~FileHandler() {
    if (this->src.is_open()) {
        this->src.close();
    }

    if (this->dst.is_open()) {
        this->dst.close();
    }
}