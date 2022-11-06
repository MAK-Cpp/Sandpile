#define GIF

#ifndef GIF
#include "bmppicture.h"
#endif
#include "sandpile.h"

#ifdef GIF
#include "gif_animation.h"
#endif

#include <queue>
#include <map>
#include <unordered_map>

void HelpWithUsage() {
    std::cout << "Usage: Labwork3.exe \n"
    << "[-l | --length=<value>]\n"
    << "[-w | --width=<value>]\n"
    << "[-i | --input=<path/to/file>]\n"
    << "[-o | --output=<path/to/directory>]\n"
    << "[-m | --max-iter=<value>]\n"
    << "[-f | --freq=<value>]";
}

void ErrorFlag(const std::string& name) {
    std::cout << "ERROR: flag " << name << " does not exist";
}

void ErrorPath(const std::filesystem::path& path) {
    std::cout << "ERROR: path " << path << " dose not exist";
}

void ErrorDirectory(const std::filesystem::path& path) {
    std::cout << "ERROR: path " << path << " is not a directory";
}

void ErrorFile(const std::filesystem::path& path) {
    std::cout << "ERROR: path " << path << " is not a .tsv file";
}

namespace const_color {
    const BmpPicture::RGBQUAD kWhite = {255, 255, 255};
    const BmpPicture::RGBQUAD kGreen = {0, 255, 0};
    const BmpPicture::RGBQUAD kPurple = {255, 0, 255};
    const BmpPicture::RGBQUAD kYellow = {255, 255, 0};
    const BmpPicture::RGBQUAD kBlack = {0, 0, 0};
}

struct Flags {
    uint16_t length;
    uint16_t width;
    std::filesystem::path input;
    std::filesystem::path output;
    uint64_t max_iter;
    uint64_t freq;
    Flags() {
        length = 0;
        width = 0;
        input = "";
        output = "";
        max_iter = 0;
        freq = 0;
    }
};


void HeapCollapse(int32_t x, int32_t y, SandPile& sand, std::queue <std::pair <int32_t, int32_t>>& new_changes, BmpPicture& image) {
    BmpPicture::RGBQUAD pixel = image.GetPixel(x, y);
    if (sand.array[x][y] == nullptr) {
        pixel.Upgrade();
        image.ChangePixel(x, y, pixel);
        if (pixel == const_color::kBlack) {
            sand.array[x][y] = new uint64_t(4);
            new_changes.push({x, y});
        }
    } else {
        (*sand.array[x][y])++;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        HelpWithUsage();
        return 0;
    }

    Flags flags;
    


    for (uint16_t i = 1; i < argc; ++i) {
        std::string flag_name = "";
        std::string arg = argv[i];
        uint16_t j = 0;
        for (; arg[j] != '=' && arg[j] != '\0'; j++) {
            flag_name += arg[j];
        }
        if (arg[j] == '\0' || flag_name.size() < 2 || flag_name[0] != '-') {
            ErrorFlag(flag_name);
            return -1;
        } 
        if (flag_name[1] == '-') {
            if (flag_name == "--length") {
                j++;
                flags.length = std::stoi(argv[i] + j);
            } else if (flag_name == "--width") {
                j++;
                flags.width = std::stoi(argv[i] + j);
            } else if (flag_name == "--input") {
                std::string path = "";
                j++;
                for (; arg[j] != '\0'; j++) {
                    path += arg[j];
                }
                if (!std::filesystem::exists(path)) {
                    ErrorPath(path);
                    return -2;
                } else if (!std::filesystem::is_regular_file(path) || std::filesystem::path(path).extension() != ".tsv") {
                    ErrorFile(path);
                    return -2;
                } 
                flags.input = path;
            } else if (flag_name == "--output") {
                std::string path = "";
                j++;
                for (; arg[j] != '\0'; j++) {
                    path += arg[j];
                }
                if (!std::filesystem::exists(path)) {
                    ErrorPath(path);
                    return -2;
                } else if (!std::filesystem::is_directory(path)) {
                    ErrorDirectory(path);
                    return -2;
                }
                flags.output = path;
            } else if (flag_name == "--max-iter") {
                j++;
                flags.max_iter = std::stoull(argv[i] + j);
            } else if (flag_name == "--freq") {
                j++;
                flags.freq = std::stoull(argv[i] + j);
            } else {
                ErrorFlag(flag_name);
                return -1;
            }
        } else {
            switch (flag_name[1]) {
              case 'l': {
                j++;
                flags.length = std::stoi(argv[i] + j);
                break;
              }
              case 'w': {
                j++;
                flags.width = std::stoi(argv[i] + j);
                break;
              }
              case 'i': {
                std::string path = "";
                j++;
                for (; arg[j] != '\0'; j++) {
                    path += arg[j];
                }
                if (!std::filesystem::exists(path)) {
                    ErrorPath(path);
                    return -2;
                } else if (!std::filesystem::is_regular_file(path) || std::filesystem::path(path).extension() != ".tsv") {
                    ErrorFile(path);
                    return -2;
                } 
                flags.input = path;
                break;
              }
              case 'o': {
                std::string path = "";
                j++;
                for (; arg[j] != '\0'; j++) {
                    path += arg[j];
                }
                if (!std::filesystem::exists(path)) {
                    ErrorPath(path);
                    return -2;
                } else if (!std::filesystem::is_directory(path)) {
                    ErrorDirectory(path);
                    return -2;
                }
                flags.output = path;
                break;
              }
              case 'm': {
                j++;
                flags.max_iter = std::stoull(argv[i] + j);
                break;
              }
              case 'f': {
                j++;
                flags.freq = std::stoull(argv[i] + j);
                break;
              }
              default: {
                ErrorFlag(flag_name);
                return -1;
                break;
              }
            }
        }
    }


    #ifdef GIF
    GifFile result(flags.output, flags.width, flags.length);
    #endif

    BmpPicture image(flags.width, flags.length);

    SandPile sand(flags.width, flags.length);

    std::ifstream input;
    input.open(flags.input);
    uint16_t x, y;
    uint64_t value;
    std::queue <std::pair <int32_t, int32_t>> blacks_to_change;
    while (input >> x >> y >> value) {
        x--;
        y--;
        switch (value) {
          case 0: {
            break;
          }
          case 1: {
            image.ChangePixel(x, y, const_color::kGreen);
            break;
          }
          case 2: {
            image.ChangePixel(x, y, const_color::kPurple);
            break;
          }
          case 3: {
            image.ChangePixel(x, y, const_color::kYellow);
            break;
          }
          default: {
            image.ChangePixel(x, y, const_color::kBlack);
            sand.array[x][y] = new uint64_t(value);
            blacks_to_change.push({x, y});
            break;
          }
        }
    }
    input.close();
    #ifdef GIF
    result.WriteFrame(image.ba);
    #endif


    for (uint64_t i = 1; i <= flags.max_iter; i++) {
        if (blacks_to_change.empty() || blacks_to_change.size() == flags.length * flags.width) {
            break;
        }
        std::queue <std::pair <int32_t, int32_t>> new_changes;
        while (!blacks_to_change.empty()) {
            BmpPicture::RGBQUAD pixel;
            int32_t y = blacks_to_change.front().second;
            int32_t x = blacks_to_change.front().first;
            blacks_to_change.pop();
            if (y - 1 >= 0) {
                HeapCollapse(x, y - 1, sand, new_changes, image);
                (*sand.array[x][y])--;
            }
            if (x - 1 >= 0) {
                HeapCollapse(x - 1, y, sand, new_changes, image);
                (*sand.array[x][y])--;
            } 
            if (y + 1 < image.bih.biWidth) {
                HeapCollapse(x, y + 1, sand, new_changes, image);
                (*sand.array[x][y])--;
            }
            if (x + 1 < image.bih.biHeight) {
                HeapCollapse(x + 1, y, sand, new_changes, image);
                (*sand.array[x][y])--;
            }
            
            if ((*sand.array[x][y]) >= 4) {
                new_changes.push({x, y});
            } else {
                switch ((*sand.array[x][y])) {
                case 0: {
                    image.ChangePixel(x, y, const_color::kWhite);
                    break;
                }
                case 1: {
                    image.ChangePixel(x, y, const_color::kGreen);
                    break;
                }
                case 2: {
                    image.ChangePixel(x, y, const_color::kPurple);
                    break;
                }
                case 3: {
                    image.ChangePixel(x, y, const_color::kYellow);
                    break;
                }
                default: {
                    break;
                }
                
                }
                delete sand.array[x][y];
                sand.array[x][y] = nullptr;
            }
        }
        blacks_to_change = new_changes;

        
        
        if (flags.freq != 0 && i % flags.freq == 0) {
            image.CreateImage(flags.output, i);
        }
        #ifdef GIF
        if (flags.freq != 0 && i % 1000 == 0) {
            result.WriteFrame(image.ba);
        }
        #endif
    }

   
    if (flags.freq == 0) {
        image.CreateImage(flags.output, flags.max_iter);
    }
    
    image.CreateImage(flags.output, 0);
    #ifdef GIF
    result.WriteFrame(image.ba);
    #endif
    #ifdef GIF
    result.End();
    #endif
    return 0;
}