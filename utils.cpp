#include "utils.h"
#include <cctype>
#include <sstream>

std::string Trim(const std::string& str) {
    std::size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }

    std::size_t end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        --end;
    }

    return str.substr(start, end - start);
}

std::vector<std::string> Split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, delimiter)) {
        std::string trimmed = Trim(token);
        if (!trimmed.empty()) {
            result.push_back(trimmed);
        }
    }

    return result;
}