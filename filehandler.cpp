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

    for (int i = 0; i < abs_width * abs_height; i++) {
        Pixel pixel{};
        this->src.read(reinterpret_cast<char *>(&pixel), sizeof(Pixel));
        this->image.push_back(pixel);
    }
}

const BMPFIleHeader &FileHandler::getFileHeader() const {
    return this->bfh;
}

const BMPInfoHeader &FileHandler::getInfoHeader() const {
    return this->bih;
}

std::vector<Pixel> &FileHandler::getImage() {
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
    this->dst.write(reinterpret_cast<char *>(this->image.data()), this->image.size() * sizeof(Pixel));
}

FileHandler::~FileHandler() {
    if (this->src.is_open()) {
        this->src.close();
    }

    if (this->dst.is_open()) {
        this->dst.close();
    }
}