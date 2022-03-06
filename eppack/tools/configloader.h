#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

namespace configloader{
    std::vector<std::string> spliter(const std::string &str, const std::string &pattern);
    bool string2bool(std::string s);

    struct configData{
        bool cli_workspace, cli_nameshow, cli_exittip, cli_errorhighlight;
    };

    class Loader{
        configData data;
    public:
        Loader(std::string path);
        configData getData();
    };
}