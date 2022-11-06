#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "bmppicture.h"
#include "gif.h"

struct GifFile {
    GifWriter g;
    int32_t height, width, delay;
    GifFile(std::filesystem::path output, const int32_t width = 1920, const int32_t height = 1080, const int32_t delay = 6) {
        std::string name_gif = "result.gif";
        this->height = height;
        this->width = width;
        this->delay = delay;
        output.append(name_gif);
        char gif_filename[output.string().size() + 1];
        for (int32_t i = 0; i < output.string().size(); i++) {
            gif_filename[i] = output.string()[i];
        }
        gif_filename[output.string().size()] = '\0';
        GifBegin(&g, gif_filename, width, height, delay);
    }
    void WriteFrame(BmpPicture::BitMapArray img) {
        std::vector <uint8_t> pixels (height * width * 4);
        for (int32_t i = 0; i < height * width; i++) {
            pixels[i * 4] = img.Array[height - i / width - 1][i % width].rgbRed;
            pixels[i * 4 + 1] = img.Array[height - i / width - 1][i % width].rgbGreen;
            pixels[i * 4 + 2] = img.Array[height - i / width - 1][i % width].rgbBlue;
            pixels[i * 4 + 3] = 0;

        }
        GifWriteFrame(&g, pixels.data(), width, height, delay);
    }
    void End() {
        GifEnd(&g);
    }
};