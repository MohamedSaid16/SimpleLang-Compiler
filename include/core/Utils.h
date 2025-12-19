#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Error.h"

namespace Utils {
    // String utilities
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    bool startsWith(const std::string& str, const std::string& prefix);
    bool endsWith(const std::string& str, const std::string& suffix);
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);
    
    // Type utilities
    std::string tokenTypeToString(TokenType type);
    std::string valueTypeToString(const std::variant<int, float, bool, std::string>& value);
    
    // File utilities
    std::string readFile(const std::string& filename);
    bool writeFile(const std::string& filename, const std::string& content);
    
    // Formatting
    template<typename... Args>
    std::string format(const std::string& format, Args... args) {
        size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
        std::vector<char> buf(size);
        snprintf(buf.data(), size, format.c_str(), args...);
        return std::string(buf.data(), buf.data() + size - 1);
    }
    
    // Error handling
    void printError(const Error& error);
    void printErrors(const std::vector<Error>& errors);
};

#endif