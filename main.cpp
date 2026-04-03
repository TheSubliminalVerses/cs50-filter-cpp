#include <iostream>
#include <vector>
#include <format>

#include "include/filehandler.hpp"
#include "include/filter.hpp"

int main(const int argc, const char *argv[]) {
    if (const int arguments = argc - 1; arguments < 3) {
        std::cerr << "Invalid arg length!" << std::endl;
        return 1;
    }

    if (argv[1] == nullptr || argv[2] == nullptr || argv[3] == nullptr) {
        std::cerr << "Arguments can't be null!" << std::endl;
        return 1;
    }

    const std::string flag(argv[1]);
    const std::string input(argv[2]);
    const std::string output(argv[3]);

    auto handler = FileHandler(input);

    std::vector<std::vector<Pixel>> &image = handler.getImage();

    switch (flag[1]) {
        case 'g':
            toGrayScale(image);
            break;
        case 's':
            toSepia(image);
            break;
        case 'b':
            try {
                blur(image, 1);
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
            break;
        case 'e':
            try {
                edgeDetection(image, 1);
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
            break;
        case 'd':
            if (argv[4] == nullptr) {
                std::cerr << "File is required!" << std::endl;
                return 1;
            }

            try {
                dither(image, "palette.csv");
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
            break;
        default:
            std::cerr << "Invalid flag!" << std::endl;
            return 1;
    }

    handler.openOutput(output);
    handler.writeHeaders();
    handler.writeImage();

    std::cout << "Success!" << std::endl;
}
