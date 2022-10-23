#include "bmppicture.h"
#include "sandpile.h"




int main(int argc, char* argv[]) {
    // ---> 1 байт, можно файл побайтово читать <---

    BmpPicture check;
    check.CreateImage("C:\\Users\\User\\Desktop\\c++\\ITMO\\BaseOfProgramming\\labwork-3-N4R1CK\\imgs", 0);
    // uint16_t a = 0x4D42;
    // std::cout << sizeof(check.bmh);
    // char x = static_cast<char>(a), y = static_cast<char>(a >> 8);
    // std::cout << (uint16_t)y << ' ' <<  (uint16_t)x;
    return 0;
}