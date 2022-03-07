#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include "../error/epperr.h"

namespace configloader{
    std::vector<std::string> spliter(const std::string &str, const std::string &pattern);
    bool string2bool(std::string s);

    struct configData{
        bool cli_workspace, cli_nameshow, cli_exittip, cli_errorhighlight;
        bool debug_echoast, debug_echotokeng;
    };

    class Loader{
        configData data;
    public:
        Loader(std::string path);
        configData getData();
    };
}