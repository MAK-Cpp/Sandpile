#include <iostream>

struct SandPile {
    uint64_t*** array;
    SandPile(const int32_t& width = 1920, const int32_t& height = 1080) {
        array = new uint64_t**[height];
        for (int32_t i = 0; i < height; i++) {
            array[i] = new uint64_t*[width];
            for (int32_t j = 0; j < width; j++) {
                array[i][j] = nullptr;
            }
        }
    }
};