#include "configloader.h"

std::vector<std::string> configloader::spliter(const std::string &str, const std::string &pattern){
    char *strc = new char[strlen(str.c_str()) + 1];
    strcpy(strc, str.c_str());
    std::vector<std::string> resultVec;
    char *tmpStr = strtok(strc, pattern.c_str());
    while (tmpStr != NULL)
    {
        resultVec.push_back(std::string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str());
    }
    delete[] strc;

    return resultVec;
}
bool configloader::string2bool(std::string s){
    if(s == "true") return true;
    else if(s == "false") return false;
    else return false;
}

configloader::Loader::Loader(std::string path){
    typedef std::string cfgKey;
    typedef std::string cfgValue;
    std::ifstream ecfg(path);
    std::istreambuf_iterator<char> begin(ecfg);
    std::istreambuf_iterator<char> end;
    std::string content(begin, end);
    std::vector<std::pair<cfgKey, cfgValue>> configDict;
    auto temp = spliter(content, "\n");
    for(auto term: temp){
        std::pair<cfgKey, cfgValue> configTerm;
        configTerm.first = spliter(term, "=")[0];
        configTerm.second = spliter(term, "=")[1];
        configDict.push_back(configTerm);
    }
    for(auto term: configDict){
        if(term.first == "cli-workspace") data.cli_workspace = string2bool(term.second);
        else if(term.first == "cli-exittip") data.cli_exittip = string2bool(term.second);
        else if(term.first == "cli-errorhighlight") data.cli_errorhighlight = string2bool(term.second);
        else if(term.first == "cli-nameshow") data.cli_nameshow = string2bool(term.second);
        else throw (std::string)("'" + term.first + "'Is not a valid configuration item");
    }
}

configloader::configData configloader::Loader::getData(){
    return data;
}