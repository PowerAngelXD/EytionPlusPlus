#include "epperr.h"

epperr::Epperr::Epperr(std::string title, std::string content, int line ,int col){
    message = (std::string)("\nEytion++ encountered an exception while running:\n    {" + title + "}" + content + "\n    (in line: " + std::to_string(line) + " column: " + std::to_string(col) + ")\n");
}

epperr::Epperr::~Epperr() throw() {}

const char* epperr::Epperr::what(){
    return message.c_str();
}