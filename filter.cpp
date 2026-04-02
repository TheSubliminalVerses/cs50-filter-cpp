//
// Created by mpolesensky on 02/04/2026.
//
#include <vector>

#include "include/bmp.hpp"
#include "include/filter.hpp"


void toGrayScale(std::vector<std::vector<Pixel>> &image) {
    for (auto &row : image) {
        for (auto &[blue, green, red] : row) {
            int gray = (blue + green + red) / 3;

            gray = std::min(255, gray);

            blue = static_cast<uint8_t>(gray);
            green = static_cast<uint8_t>(gray);
            red = static_cast<uint8_t>(gray);
        }
    }
}

void toSepia(std::vector<std::vector<Pixel>> &image) {
    for (auto &row : image) {
        for (auto &[blue, green, red] : row) {
            double new_red = red * 0.393 + green * 0.769 + blue * 0.189;
            double new_green = red * 0.349 + green * 0.686 + blue * 0.168;
            double new_blue = red * 0.272 + green * 0.534 + blue * 0.131;

            new_red = std::min(255, static_cast<int>(new_red));
            new_green = std::min(255, static_cast<int>(new_green));
            new_blue = std::min(255, static_cast<int>(new_blue));

            blue = static_cast<uint8_t>(new_blue);
            green = static_cast<uint8_t>(new_green);
            red = static_cast<uint8_t>(new_red);
        }
    }
}