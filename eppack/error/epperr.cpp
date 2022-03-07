#include "epperr.h"

epperr::Epperr::Epperr(std::string title, std::string content, int line ,int col){
    message = (std::string)("\nEytion++ encountered an exception while running:\n    {" + title + "}" + content + "\n    (in line: " + std::to_string(line) + " column: " + std::to_string(col) + ")\n");
}

epperr::Epperr::~Epperr() throw() {}

const char* epperr::Epperr::what(){
    return message.c_str();
}


epperr::EppClierr::EppClierr(std::string content){
    message = (std::string)("\nEytion++CLI encountered an exception while running:\n" +  content + "\n");
}
epperr::EppClierr::~EppClierr() throw() {}

const char* epperr::EppClierr::what(){
    return message.c_str();
}