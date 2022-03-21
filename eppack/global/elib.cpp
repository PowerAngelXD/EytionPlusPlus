#include "elib.h"

#ifdef _WIN32
void color::colprint(std::string text, color::ColorSet color, ColorSet back){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, back==ColorSet::Black?:back*16 | FOREGROUND_INTENSITY | color);
    std::cout<<text<<std::endl;
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}
#endif