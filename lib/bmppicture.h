#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <ctime>




struct BmpPicture {

    /*
    BYTE     uint8_t
    WORD     uint16_t
    DWORD    uint32_t
    LONG     int32_t

    
    */

    struct BitMapFileHeader {
        uint16_t bfType; 	     // WORD    Сигнатура "BM"  Может содержать единственное значение 4D42/424D (little-endian/big-endian)
        uint32_t bfSize; 	     // DWORD 	Размер файла в байтах
        uint16_t bfReserved1;    // WORD 	Зарезервировано = 0
        uint16_t bfReserved2;	 // WORD 	Зарезервировано = 0
        uint32_t bfOffsetBits;   // DWORD   Смещение изображения от начала файла, положение пиксельных данных относительно начала данной структуры (в байтах) 	

        size_t Size() {
            return  sizeof(bfType) + 
                    sizeof(bfReserved1) + 
                    sizeof(bfReserved2) + 
                    sizeof(bfSize) + 
                    sizeof(bfOffsetBits); 
        }

        BitMapFileHeader(const int32_t& height = 1080, const int32_t& width = 1920) {
            bfType = static_cast<uint16_t>(0x4D42);
            bfReserved1 = 0;
            bfReserved2 = 0;
            bfOffsetBits = Size() + sizeof(BitMapInfoHeader);
            bfSize = bfOffsetBits + height * ((width * 3) + ((4 - (width * 3) & 3) & 3));
        }

        friend std::ofstream& operator<< (std::ofstream&, const BitMapFileHeader&);
    };
    
    struct BitMapInfoHeader {
        uint32_t biSize;
        int32_t  biWidth;
        int32_t  biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t  biXPelsPerMeter;
        int32_t  biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;

        BitMapInfoHeader(const int32_t& height = 1080, const int32_t& width = 1920) {
            biSize = sizeof(BitMapInfoHeader);
            biWidth = width;
            biHeight = height;
            biPlanes = 1;
            biBitCount = 24;             // А это самый простой формат. Здесь 3 байта определяют 3 компоненты цвета. То есть по компоненте на байт. Просто читаем по структуре RGBTRIPLE и используем его поля rgbtBlue, rgbtGreen, rgbtRed. Они идут именно в таком порядке.
            biCompression = 0x0000;     // = BI_RGB
            biSizeImage = 0;
            biXPelsPerMeter = 0;
            biYPelsPerMeter = 0;
            biClrUsed = 0;
            biClrImportant = 0;
        }

        friend std::ofstream& operator<< (std::ofstream&, const BitMapInfoHeader&);
    };

    struct RGBQUAD {
        uint8_t rgbBlue;
        uint8_t rgbRed;
        uint8_t rgbGreen;
        RGBQUAD (uint8_t r = 255, uint8_t g = 255, uint8_t b = 255) {
            rgbBlue = b;
            rgbGreen = g;
            rgbRed = r;
        }

        void Upgrade();

        friend bool operator==(BmpPicture::RGBQUAD& a, const BmpPicture::RGBQUAD& b);
        friend bool operator!=(BmpPicture::RGBQUAD& a, const BmpPicture::RGBQUAD& b);
        
    };


    struct BitMapArray {
        RGBQUAD** Array;

        BitMapArray(const int32_t& height = 1080, const int32_t& width = 1920) {
            Array = new RGBQUAD*[height];
            for (int32_t i = 0; i < height; ++i) {
                Array[i] = new RGBQUAD[width];
            }
        }
    };
    
    std::filesystem::path path_to_image;
    BitMapFileHeader bmh;
    BitMapInfoHeader bih;
    BitMapArray ba;


    
    BmpPicture (const int32_t& width = 1920, const int32_t& height = 1080) {
        bmh = *new BitMapFileHeader(height, width);
        bih = *new BitMapInfoHeader(height, width);
        ba = *new BitMapArray(height, width);
    }

    void CreateImage(std::filesystem::path, const uint32_t&);
    void ChangePixel(const int32_t& x, const int32_t& y, const RGBQUAD& color);
    RGBQUAD GetPixel(const int32_t& x, const int32_t& y);


};