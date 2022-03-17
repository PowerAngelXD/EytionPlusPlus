#include "excphg.h"

excphandling::Excp::Excp(std::string content, std::string type) {
    this->excpContent = content;
    this->excpTitle = "User defined exception[";
    if(type == "__INT__") this->excpTitle += "Type:Int";
    else if(type == "__DECI__") this->excpTitle += "Type:Deci";
    else if(type == "__STRING__") this->excpTitle += "Type:String";
    else if(type == "__CHAR__") this->excpTitle += "Type:Char";
    else if(type == "__BOOL__") this->excpTitle += "Type:Bool";
    this->excpTitle += "]";
}