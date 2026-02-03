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


std::string StringHelpers::join(
    const std::vector<std::string>& to_join, const std::string& join_with) {

    std::string output;

    for (size_t i = 0; i < to_join.size(); i++) {
        output += to_join.at(i);
        if (i < to_join.size() - 1) {
            output += join_with;
        }
    }

    return output;
}

std::string StringHelpers::padToMulitple(const std::string& to_pad, int width, const char& pad_with) {
    // int new_length = width;
    // while (new_length < to_pad.length()) {
    //     new_length += width;
    // } 

    int new_length = std::ceil(to_pad.length() / static_cast<float>(width)) * width;

    return leftAlign(to_pad, new_length, pad_with);
}
