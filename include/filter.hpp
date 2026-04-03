//
// Created by mpolesensky on 02/04/2026.
//

#ifndef FILTER_FILTER_HPP
#define FILTER_FILTER_HPP

#include <vector>
#include "bmp.hpp"

// Turn image into grayscale
void toGrayScale(std::vector<std::vector<Pixel>> &image);

// Turn image into sepia
void toSepia(std::vector<std::vector<Pixel>> &image);

// Apply blug to the image
void blur(std::vector<std::vector<Pixel>> &image, int radius);

// Apply the sobel operator
void edgeDetection(std::vector<std::vector<Pixel>> &image, int radius);

// Apply dithering
void dither(std::vector<std::vector<Pixel>> &image, const std::string &config);

#endif //FILTER_FILTER_HPP
