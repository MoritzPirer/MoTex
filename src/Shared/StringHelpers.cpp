#include "../../inc/Shared/StringHelpers.hpp"

std::string StringHelpers::leftAlign(const std::string& str,
    unsigned int total_width, const char& pad_with) {

    if (str.length() >= total_width) {
        return str.substr(0, total_width);
    }

    unsigned int padding_needed = total_width - str.length();
    std::string output = str;
    output += std::string(padding_needed, pad_with); 

    return output; 
}

std::string StringHelpers::rightAlign(const std::string& str,
    unsigned int total_width, const char& pad_with) {

    if (str.length() >= total_width) {
        return str.substr(0, total_width);
    }

    unsigned int padding_needed = total_width - str.length();
    std::string output(padding_needed, pad_with);
    output += str;

    return output; 
}