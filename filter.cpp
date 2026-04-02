//
// Created by mpolesensky on 02/04/2026.
//
#include <vector>

#include "include/bmp.hpp"
#include "include/filter.hpp"


void toGrayScale(std::vector<Pixel> &image) {
    for (auto &[blue, green, red] : image) {
        int gray = (blue + green + red) / 3;

        gray = std::min(255, gray);

        blue = static_cast<uint8_t>(gray);
        green = static_cast<uint8_t>(gray);
        red = static_cast<uint8_t>(gray);
    }
}