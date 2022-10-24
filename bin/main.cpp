#include "bmppicture.h"
#include "sandpile.h"

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
    

    BmpPicture check(flags.width, flags.length);
    for (int i = 0; i < check.bih.biWidth; i++) {
        for (int j = check.bih.biHeight / 2 - 100; j <= check.bih.biHeight / 2 + 100; j++) {
            check.ChangePixel(j, i, colors.kBlack);
        }
        
    }
    // check.CreateImage("C:\\Users\\User\\Desktop\\c++\\ITMO\\BaseOfProgramming\\labwork-3-N4R1CK\\imgs");
    return 0;
}