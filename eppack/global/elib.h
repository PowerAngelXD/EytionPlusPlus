#include <iostream>
#include <vector>
#include <functional>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>

namespace color{
    enum ColorSet{
        Black=0, Blue, Green, Lightgreen, Red, Purple, Yellow, White, Grey, Lightblue, Brightgreen, 
        Brightergreen, Lightred, Lightpurple, Lightyellow, Lightwhite
    };

    void colprint(std::string text, ColorSet color, ColorSet back=ColorSet::Black);
}

#endif