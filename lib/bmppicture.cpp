#include "bmppicture.h"

void Write(std::ofstream& image, const uint8_t& num) {
    char buff = static_cast<char>(num);
    image.write(&buff, 1);
}

void Write(std::ofstream& image, uint16_t num) {
    char buff[] = {static_cast<char>(num), static_cast<char>(num >> 8)};
    image.write(buff, 2);
}

void Write(std::ofstream& image, uint32_t num) {
    char buff[] = {static_cast<char>(num), static_cast<char>(num >> 8), static_cast<char>(num >> 16), static_cast<char>(num >> 24)};
    image.write(buff, 4);
}

void Write(std::ofstream& image, int32_t num) {
    char buff[] = {static_cast<char>(num), static_cast<char>(num >> 8), static_cast<char>(num >> 16), static_cast<char>(num >> 24)};
    image.write(buff, 4);
}

void Write(std::ofstream& image, BmpPicture::RGBQUAD color) {
    char buff[] = {static_cast<char>(color.rgbBlue), static_cast<char>(color.rgbGreen), static_cast<char>(color.rgbRed)};
    image.write(buff, 3);
}


std::ofstream& operator<< (std::ofstream& op, const BmpPicture::BitMapFileHeader& x) {
    Write(op, x.bfType);
    Write(op, x.bfSize);
    Write(op, x.bfReserved1);
    Write(op, x.bfReserved2);
    Write(op, x.bfOffsetBits);
    return op;
}

std::ofstream& operator<< (std::ofstream& op, const BmpPicture::BitMapInfoHeader& x) {
    Write(op, x.biSize);
    Write(op, x.biWidth);
    Write(op, x.biHeight);
    Write(op, x.biPlanes);
    Write(op, x.biBitCount);
    Write(op, x.biCompression);
    Write(op, x.biSizeImage);
    Write(op, x.biXPelsPerMeter);
    Write(op, x.biYPelsPerMeter);
    Write(op, x.biClrUsed);
    Write(op, x.biClrImportant);
    return op;
}


void BmpPicture::CreateImage(std::filesystem::path directory, const uint32_t& iteration) {
    if (!std::filesystem::exists(directory)) {
        std::cout << "ERROR: no such file or directory\n";
        exit(-1);
    }
    const time_t tm = time(nullptr);
    char buf[64];
    strftime(buf, std::size(buf), "_%d_%m_%Y_%H_%M_%S", localtime(&tm));
    std::string image_name = "image" + std::to_string(iteration) + buf + ".bmp";
    directory.append(image_name);
    std::ofstream image(directory, std::ios::binary);
    std::cout << directory << '\n';

    image << bmh;
    image << bih;
    for (int32_t i = 0; i < bih.biHeight; ++i) {
        for (int32_t j = 0; j < bih.biWidth; ++j) {
            Write(image, ba.Array[i][j]);
        }
        const uint8_t kZero = 0;
        for (int32_t j = 0; j < (4 - ((bih.biWidth * 3) % 4)) * ((bih.biWidth * 3) % 4 > 0); j++) {
            Write(image, kZero);
        }
    }


    image.close();
}