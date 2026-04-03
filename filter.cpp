//
// Created by mpolesensky on 02/04/2026.
//
#include <vector>
#include <complex>
#include <utility>
#include <fstream>

#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>

#include "include/bmp.hpp"
#include "include/filter.hpp"

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;


template <Numeric T, Numeric U>
bool isCastable(T value) {
    return std::cmp_less_equal(value, std::numeric_limits<U>::max());
}

Pixel findClosestPaletteColor(const Pixel targetColor, const std::vector<Pixel> &palette) {
    int shortestDistance = INT32_MAX;
    Pixel closesColor = palette[0];

    for (const auto &color : palette) {
        const int dR = color.red - targetColor.red;
        const int dG = color.green - targetColor.green;
        const int dB = color.blue - targetColor.blue;

        int distanceSQ = (dR * dR) + (dG * dG) + (dB * dB);

        if (distanceSQ < shortestDistance) {
            shortestDistance = distanceSQ;
            closesColor = color;
        }
    }

    return closesColor;
}


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

    int height = 0;
    int width = 0;

    if (isCastable<unsigned long, int>(image.size())) {
        height = static_cast<int>(image.size());
    } else {
        throw std::overflow_error("Vector size is too large!");
    }

    if (isCastable<unsigned long, int>(image[0].size())) {
        width = static_cast<int>(image[0].size());
    } else {
        throw std::overflow_error("Vector size is too large!");
    }

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

            avgBlue = std::min(255, avgBlue);
            avgGreen = std::min(255, avgGreen);
            avgRed = std::min(255, avgRed);

            image[i][j].blue = static_cast<uint8_t>(avgBlue);
            image[i][j].green = static_cast<uint8_t>(avgGreen);
            image[i][j].red = static_cast<uint8_t>(avgRed);
        }
    }
}

void edgeDetection(std::vector<std::vector<Pixel> > &image, const int radius) {
    auto imgCpy = image;

    int height = 0;
    int width = 0;

    if (isCastable<unsigned long, int>(image.size())) {
        height = static_cast<int>(image.size());
    } else {
        throw std::overflow_error("Vector size is too large!");
    }

    if (isCastable<unsigned long, int>(image[0].size())) {
        width = static_cast<int>(image[0].size());
    } else {
        throw std::overflow_error("Vector size is too large!");
    }

    const std::vector<std::vector<int>> kx = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
    const std::vector<std::vector<int>> ky = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            int sumBlueX = 0;
            int sumGreenX = 0;
            int sumRedX = 0;

            int sumBlueY = 0;
            int sumGreenY = 0;
            int sumRedY = 0;

            for (int y = -radius; y <= radius; y++) {
                for (int x = -radius; x <= radius; x++) {
                    const int nx = x + j;
                    const int ny = y + i;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        const auto [blue, green, red] = imgCpy[ny][nx];

                        sumBlueX = sumBlueX + blue * kx[y + 1][x + 1];
                        sumGreenX = sumGreenX + green * kx[y + 1][x + 1];
                        sumRedX = sumRedX + red * kx[y + 1][x + 1];

                        sumBlueY = sumBlueY + blue * ky[y + 1][x + 1];
                        sumGreenY = sumGreenY + green * ky[y + 1][x + 1];
                        sumRedY = sumRedY + red * ky[y + 1][x + 1];

                        double blueMag = std::sqrt(std::pow(sumBlueX, 2) + std::pow(sumBlueY, 2));
                        double greenMag = std::sqrt(std::pow(sumGreenX, 2) + std::pow(sumGreenY, 2));
                        double redMag = std::sqrt(std::pow(sumRedX, 2) + std::pow(sumRedY, 2));

                        blueMag = std::min(255, static_cast<int>(blueMag));
                        greenMag = std::min(255, static_cast<int>(greenMag));
                        redMag = std::min(255, static_cast<int>(redMag));

                        image[i][j].blue = static_cast<uint8_t>(blueMag);
                        image[i][j].green = static_cast<uint8_t>(greenMag);
                        image[i][j].red = static_cast<uint8_t>(redMag);
                    }
                }
            }
        }
    }
}

void dither(std::vector<std::vector<Pixel> > &image, const std::string &config) {
    // TODO: Write a smol rust script
    lua_State *L = luaL_newstate();

    luaL_openlibs(L);
    lua_pushstring(L, config.c_str());
    lua_setglobal(L, "CONFIG");
    // TODO: Finish implementing lua

    int width = 0;
    int height = 0;

    if (isCastable<unsigned long, int>(image.size())) {
        height = static_cast<int>(image.size());
    } else {
        throw std::overflow_error("Vector size is too large!");
    }

    if (isCastable<unsigned long, int>(image[0].size())) {
        width = static_cast<int>(image[0].size());
    } else {
        throw std::overflow_error("Vector size is too large!");
    }

    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            Pixel oldPixel = image[i][j];
            Pixel newPixel = findClosestPaletteColor(oldPixel, palette);

            image[i][j] = newPixel;

            uint8_t errBlue = 0;
            uint8_t errGreen = 0;
            uint8_t errRed = 0;

            if (isCastable<int, uint8_t>(oldPixel.blue - newPixel.blue)) {
                errBlue = static_cast<uint8_t>(oldPixel.blue- newPixel.blue);
            } else {
                throw std::overflow_error("Color value is too large!");
            }

            if (isCastable<int, uint8_t>(oldPixel.green - newPixel.green)) {
                errGreen = static_cast<uint8_t>(oldPixel.green - newPixel.green);
            } else {
                throw std::overflow_error("Color value is too large!");
            }

            if (isCastable<int, uint8_t>(oldPixel.red - newPixel.red)) {
                errRed = static_cast<uint8_t>(oldPixel.red- newPixel.red);
            } else {
                throw std::overflow_error("Color value is too large!");
            }

            Pixel error = { errBlue, errGreen, errRed };

            auto spreadError = [&](const int nx, const int ny, const int weight) {
                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    image[ny][nx].blue = image[ny][nx].blue + error.blue * weight;
                    image[ny][nx].green = image[ny][nx].green + error.green * weight;
                    image[ny][nx].red = image[ny][nx].red + error.red * weight;
                }
            };

            spreadError(j + 1, i, 7/16);
            spreadError(j - 1, i + 1, 3/16);
            spreadError(j, i + 1, 5/16);
            spreadError(j + 1, i + 1, 1/16);
        }
    }
}