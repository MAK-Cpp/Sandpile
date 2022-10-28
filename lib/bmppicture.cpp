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

bool operator==(BmpPicture::RGBQUAD& a, const BmpPicture::RGBQUAD& b) {
    return a.rgbBlue == b.rgbBlue && a.rgbGreen == b.rgbGreen && a.rgbRed == b.rgbRed;
}
bool operator!=(BmpPicture::RGBQUAD& a, const BmpPicture::RGBQUAD& b) {
    return !(a == b);
}

void BmpPicture::RGBQUAD::Upgrade() {
    if (this->rgbRed == 255 && this->rgbGreen == 255 && this->rgbBlue == 255) {
        this->rgbRed = 0;
        this->rgbGreen = 255;
        this->rgbBlue = 0;
    } else if (this->rgbRed == 0 && this->rgbGreen == 255 && this->rgbBlue == 0) {
        this->rgbRed = 255;
        this->rgbGreen = 0;
        this->rgbBlue = 255;
    } else if (this->rgbRed == 255 && this->rgbGreen == 0 && this->rgbBlue == 255) {
        this->rgbRed = 255;
        this->rgbGreen = 255;
        this->rgbBlue = 0;
    } else if (this->rgbRed == 255 && this->rgbGreen == 255 && this->rgbBlue == 0) {
        this->rgbRed = 0;
        this->rgbGreen = 0;
        this->rgbBlue = 0;
    }
}

void BmpPicture::CreateImage(std::filesystem::path directory, const uint32_t& iterator) {
    if (!std::filesystem::exists(directory)) {
        std::cout << "ERROR: no such file or directory\n";
        exit(-1);
    }
    const time_t tm = time(nullptr);
    char buf[64];
    strftime(buf, std::size(buf), "_%d.%m.%Y_%H-%M-%S", localtime(&tm));
    std::string image_name = "image";
    image_name += buf;
    image_name += "_" + std::to_string(iterator);
    image_name += ".bmp";
    directory.append(image_name);
    std::ofstream image(directory, std::ios::binary);
    // std::cout << directory << '\n';

    image << bmh;
    image << bih;
    for (int32_t i = 0; i < bih.biHeight; ++i) {
        for (int32_t j = 0; j < bih.biWidth; ++j) {
            Write(image, ba.Array[i][j]);
        }
        const uint8_t kZero = 0;
        for (int32_t j = 0; j < ((4 - ((bih.biWidth * 3) & 3)) & 3); j++) {
            Write(image, kZero);
        }
    }


    image.close();
}

void BmpPicture::ChangePixel(const int32_t& x, const int32_t& y, const BmpPicture::RGBQUAD& color) {
    ba.Array[x][y] = color;
}

BmpPicture::RGBQUAD BmpPicture::GetPixel(const int32_t& x, const int32_t& y) {
    return ba.Array[x][y];
}