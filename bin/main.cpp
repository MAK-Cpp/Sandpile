#include "bmppicture.h"
#include <queue>
#include <map>
#include <unordered_map>

void HelpWithUsage() {
    std::cout << "Usage: Labwork3.exe [-l | --length=<value>] [-w | --width=<value>] [-i | --input=<path/to/file>] [-o | --output=<path/to/directory>] [-m | --max-iter=<value>] [-f | --freq=<value>]";
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

struct Colors {
    const BmpPicture::RGBQUAD kWhite = {255, 255, 255};
    const BmpPicture::RGBQUAD kGreen = {0, 255, 0};
    const BmpPicture::RGBQUAD kPurple = {255, 0, 255};
    const BmpPicture::RGBQUAD kYellow = {255, 255, 0};
    const BmpPicture::RGBQUAD kBlack = {0, 0, 0};
    Colors() = default;
};

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


void HeapCollapse(uint16_t x, uint16_t y,std::unordered_map <uint64_t, uint32_t>& delta, std::unordered_map <uint64_t, uint32_t>& new_delta, BmpPicture& image) {
    Colors colors;
    BmpPicture::RGBQUAD pixel = image.GetPixel(x, y);
    if (pixel != colors.kBlack) {
        pixel.Upgrade();
        image.ChangePixel(x, y, pixel);
        if (pixel == colors.kBlack) {
            uint64_t coordinates = (static_cast<uint64_t>(x) << 32) + y;
            new_delta[coordinates] += 4;
        }
    } else {
        uint64_t coordinates = (static_cast<uint64_t>(x) << 32) + y;
        if (delta[coordinates] != 0) {
            delta[coordinates]++;
            
        } else {
            delta.erase(coordinates);
            new_delta[coordinates]++;
        }   
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
                uint16_t value = 0;
                j++;
                for (; arg[j] != '\0'; j++) {
                    value = value * 10 + (arg[j] - '0');
                }
                flags.length = value;
            } else if (flag_name == "--width") {
                uint16_t value = 0;
                j++;
                for (; arg[j] != '\0'; j++) {
                    value = value * 10 + (arg[j] - '0');
                }
                flags.width = value;
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
                uint64_t value = 0;
                j++;
                for (; arg[j] != '\0'; j++) {
                    value = value * 10 + (arg[j] - '0');
                }
                flags.max_iter = value;
            } else if (flag_name == "--freq") {
                uint64_t value = 0;
                j++;
                for (; arg[j] != '\0'; j++) {
                    value = value * 10 + (arg[j] - '0');
                }
                flags.freq = value;
            } else {
                ErrorFlag(flag_name);
                return -1;
            }
        } else {
            switch (flag_name[1]) {
              case 'l': {
                uint16_t value = 0;
                j++;
                for (; arg[j] != '\0'; j++) {
                    value = value * 10 + (arg[j] - '0');
                }
                flags.length = value;
                break;
              }
              case 'w': {
                uint16_t value = 0;
                j++;
                for (; arg[j] != '\0'; j++) {
                    value = value * 10 + (arg[j] - '0');
                }
                flags.width = value;
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
                uint64_t value = 0;
                j++;
                for (; arg[j] != '\0'; j++) {
                    value = value * 10 + (arg[j] - '0');
                }
                flags.max_iter = value;
                break;
              }
              case 'f': {
                uint64_t value = 0;
                j++;
                for (; arg[j] != '\0'; j++) {
                    value = value * 10 + (arg[j] - '0');
                }
                flags.freq = value;
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

    Colors colors;
    BmpPicture image(flags.width, flags.length);

    std::unordered_map <uint64_t, uint32_t> delta;
    // координаты теперь будут храниться по 32 бита на каждую -> 64 бита всего
    std::ifstream input;
    input.open(flags.input);
    uint16_t x, y;
    uint64_t value;
    uint64_t cnt_black_pixels = 0;
    while (input >> x >> y >> value) {
        switch (value) {
          case 0: {
            break;
          }
          case 1: {
            image.ChangePixel(x, y, colors.kGreen);
            break;
          }
          case 2: {
            image.ChangePixel(x, y, colors.kPurple);
            break;
          }
          case 3: {
            image.ChangePixel(x, y, colors.kYellow);
            break;
          }
          default: {
            uint64_t coordinates = (static_cast<uint64_t>(x) << 32) + y;
            image.ChangePixel(x, y, colors.kBlack);
            delta[coordinates] = value;
            cnt_black_pixels++;
            break;
          }
        }
    }
    input.close();


    for (uint64_t i = 1; i <= flags.max_iter; i++) {
        if (delta.empty()) {
            break;
        }
        std::unordered_map <uint64_t, uint32_t> new_delta;
        for (std::unordered_map <uint64_t, uint32_t>::iterator i = delta.begin(); i != delta.end(); i = delta.begin()) {
            BmpPicture::RGBQUAD pixel;
            int32_t y = static_cast<int32_t>(i->first);
            int32_t x = static_cast<int32_t>(i->first >> 32);
            if (y - 1 >= 0) {
                HeapCollapse(x, y - 1, delta, new_delta, image);
                i->second--;
            }
            if (x - 1 >= 0) {
                HeapCollapse(x - 1, y, delta, new_delta, image);
                i->second--;
            } 
            if (y + 1 < image.bih.biWidth) {
                HeapCollapse(x, y + 1, delta, new_delta, image);
                i->second--;
            }
            if (x + 1 < image.bih.biHeight) {
                HeapCollapse(x + 1, y, delta, new_delta, image);
                i->second--;
            }
            
            if (i->second >= 4) {
                new_delta[i->first] += i->second;
            } else {
                switch (i->second) {
                case 0: {
                    image.ChangePixel(x, y, colors.kWhite);
                    break;
                }
                case 1: {
                    image.ChangePixel(x, y, colors.kGreen);
                    break;
                }
                case 2: {
                    image.ChangePixel(x, y, colors.kPurple);
                    break;
                }
                case 3: {
                    image.ChangePixel(x, y, colors.kYellow);
                    break;
                }
                default: {
                    break;
                }
                
                }
                new_delta.erase(i->first);
            }
            delta.erase(i->first);
        }
        delta = new_delta;
        new_delta.clear();

        
        
        if (flags.freq != 0 && i % flags.freq == 0) {
            image.CreateImage(flags.output, i);
        }
    }

   
    if (flags.freq == 0) {
        image.CreateImage(flags.output, flags.max_iter);
    }
    
    return 0;
}