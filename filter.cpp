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

void blur(std::vector<std::vector<Pixel>> &image, const int radius) {
    const auto imgCpy = image;

    const int kSize = 2 * radius + 1;
    const int numPix = kSize * kSize;

    const int height = image.size();
    const int width = image[0].size();

    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;

            for (int kx = -1; kx <= 1; kx++) {
                for (int ky = -1; ky <= 1; ky++) {
                    const int nx = j + kx;
                    const int ny = i + ky;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        sumBlue += imgCpy[ny][nx].blue;
                        sumGreen += imgCpy[ny][nx].green;
                        sumRed += imgCpy[ny][nx].red;
                    }
                }
            }

            int avgBlue = sumBlue / numPix;
            int avgGreen = sumGreen / numPix;
            int avgRed = sumRed / numPix;

            avgBlue =std::min(255, avgBlue);
            avgGreen = std::min(255, avgGreen);
            avgRed = std::min(255, avgRed);

            image[i][j].blue = static_cast<uint8_t>(avgBlue);
            image[i][j].green = static_cast<uint8_t>(avgGreen);
            image[i][j].red = static_cast<uint8_t>(avgRed);
        }
    }
}