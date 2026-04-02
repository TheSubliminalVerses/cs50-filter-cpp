//
// Created by mpolesensky on 02/04/2026.
//

#ifndef FILTER_BMP_HPP
#define FILTER_BMP_HPP

#include <cstdint>

struct BMPFIleHeader {
    uint16_t bfType{0x4d42};
    uint32_t bfSize{0};
    uint16_t bfReserved1{0};
    uint16_t bfReserved2{0};
    uint32_t bfOffBits{0};
} __attribute__((__packed__));

struct BMPInfoHeader {
    uint32_t biSize{0};
    int32_t biWidth{0};
    int32_t biHeight{0};
    uint16_t biPlanes{1};
    uint16_t biBitCount{0};
    uint32_t biCompression{0};
    uint32_t biSizeImage{0};
    int32_t biXPixelsPerMeter{0};
    int32_t biYPixelsPerMeter{0};
    uint32_t biClrUsed{0};
    uint32_t biClrImportant{0};
} __attribute__((__packed__));

struct Pixel {
    uint8_t blue{0};
    uint8_t green{0};
    uint8_t red{0};
};

#endif //FILTER_BMP_HPP
