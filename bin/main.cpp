#include "bmppicture.h"
#include "sandpile.h"
#include <queue>
#include <map>

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

struct Coordinates {
    int32_t x;
    int32_t y;
    Coordinates (int32_t x = 0, int32_t y = 0) {
        this->x = x;
        this->y = y;
    }

    friend bool operator<(const Coordinates& a, const Coordinates& b);
}; 

bool operator<(const Coordinates& a, const Coordinates& b) {
    if (a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}

struct BigSandpile {
    Coordinates cd;
    uint64_t value;
    BigSandpile(int32_t x, int32_t y, int32_t v) {
        cd = *new Coordinates(x, y);
        value = v;
    }

};

void HeapCollapse(uint16_t x, uint16_t y,std::map <Coordinates, uint32_t>& delta, std::map <Coordinates, uint32_t>& new_delta, BmpPicture& image) {
    Colors colors;
    BmpPicture::RGBQUAD pixel = image.GetPixel(x, y);
    // std::cout << i->first.x << ' ' << i->first.y - 1 << ' ' << (uint16_t)pixel.rgbRed << ' ' << (uint16_t)pixel.rgbGreen << ' ' << (uint16_t)pixel.rgbBlue << '\n';
    if (pixel != colors.kBlack) {
        pixel.Upgrade();
        image.ChangePixel(x, y, pixel);
        if (pixel == colors.kBlack) {
            Coordinates* change = new Coordinates(x, y);
            new_delta[*change] += 4;
            delete change;
        }
    } else {
        Coordinates* change = new Coordinates(x, y);
        if (delta[*change] != 0) {
            delta[*change]++;
            
        } else {
            delta.erase(*change);
            new_delta[*change]++;
        }
        
        delete change;
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
    std::cout << flags.width << '\n' << flags.length << '\n' << flags.input << '\n' << flags.output << '\n' << flags.max_iter << '\n' << flags.freq << '\n';

    Colors colors;
    BmpPicture image(flags.width, flags.length);

    std::map <Coordinates, uint32_t> delta;
    std::ifstream input;
    input.open(flags.input);
    uint16_t x, y;
    uint64_t value;
    uint64_t cnt_black_pixels = 0;
    while (input >> x >> y >> value) {
        std::cout << x << ' ' << y << ' ' << value << '\n';
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
            image.ChangePixel(x, y, colors.kBlack);
            Coordinates* change = new Coordinates(x, y);
            delta[*change] = value;
            delete change;
            cnt_black_pixels++;
            break;
          }
        }
    }
    input.close();

    image.CreateImage(flags.output, 100);

    

    for (uint64_t i = 1; i <= flags.max_iter; i++) {
        // std::cout << i << ' ' << delta.size() << '\n';
        if (delta.empty()) {
            break;
        }
        std::map <Coordinates, uint32_t> new_delta;
        for (std::map <Coordinates, uint32_t>::iterator i = delta.begin(); i != delta.end(); i = delta.begin()) {
            // std::cout << "DELTA AND NEW DELTA: " << delta.size() << ' ' << new_delta.size() << '\n';
            // std::cout << i->first.x << ' ' << i->first.y << ' ' << i->second << " -> ";
            // std::cout << i->second << ", " <<  new_delta[i->first] << " -> ";
            BmpPicture::RGBQUAD pixel;
            if (i->first.y - 1 >= 0) {
                // std::cout << "FIRST IF\n";
                HeapCollapse(i->first.x, i->first.y - 1, delta, new_delta, image);
                i->second--;
            }
            if (i->first.x - 1 >= 0) {
                // std::cout << "SECOND IF\n";
                HeapCollapse(i->first.x - 1, i->first.y, delta, new_delta, image);
                i->second--;
            } 
            if (i->first.y + 1 < image.bih.biWidth) {
                // std::cout << "THIRD IF\n";
                HeapCollapse(i->first.x, i->first.y + 1, delta, new_delta, image);
                i->second--;
            }
            if (i->first.x + 1 < image.bih.biHeight) {
                // std::cout << "FOURTH IF\n";
                HeapCollapse(i->first.x + 1, i->first.y, delta, new_delta, image);
                i->second--;
            }
            
            if (i->second >= 4) {
                new_delta[i->first] += i->second;
            } else {
                switch (i->second) {
                  case 0: {
                    image.ChangePixel(i->first.x, i->first.y, colors.kWhite);
                    break;
                  }
                  case 1: {
                    image.ChangePixel(i->first.x, i->first.y, colors.kGreen);
                    break;
                  }
                  case 2: {
                    image.ChangePixel(i->first.x, i->first.y, colors.kPurple);
                    break;
                  }
                  case 3: {
                    image.ChangePixel(i->first.x, i->first.y, colors.kYellow);
                    break;
                  }
                  default: {
                    break;
                  }
                
                }
                new_delta.erase(i->first);
            }
            // std::cout << new_delta[i->first] << '\n';
            // std::cout << i->second << '\n';
            delta.erase(i->first);
            
        }
        delta = new_delta;
        // for (auto i = delta.begin(); i != delta.end(); i++){
        //     std::cout << i->first.x << ' ' << i->first.y << ' ' << i->second << '\n';
        // }
        new_delta.clear();

        
        
        if (flags.freq != 0 && i % flags.freq == 0) {
            image.CreateImage(flags.output, i);
        }
    }

   
    if (flags.freq == 0) {
        image.CreateImage(flags.output, flags.max_iter);
    }
    image.CreateImage(flags.output, 0);
    
    return 0;
}