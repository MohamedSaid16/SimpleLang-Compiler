#include "Utils.h"
#include <fstream>
#include <algorithm>
#include <cctype>

namespace Utils {
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
    
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string item;
        
        while (std::getline(ss, item, delimiter)) {
            result.push_back(item);
        }
        
        return result;
    }
    
    bool startsWith(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() && 
               str.compare(0, prefix.size(), prefix) == 0;
    }
    
    bool endsWith(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() && 
               str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
    
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }
    
    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        return result;
    }
    
    std::string tokenTypeToString(TokenType type) {
        // This would be implemented similarly to Token::typeToString
        return "TokenType";
    }
    
    std::string valueTypeToString(const std::variant<int, float, bool, std::string>& value) {
        if (std::holds_alternative<int>(value)) return "int";
        if (std::holds_alternative<float>(value)) return "float";
        if (std::holds_alternative<bool>(value)) return "bool";
        if (std::holds_alternative<std::string>(value)) return "string";
        return "unknown";
    }
    
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        return content;
    }
    
    bool writeFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << content;
        return true;
    }
    
    void printError(const Error& error) {
        std::cout << error.toString() << std::endl;
    }
    
    void printErrors(const std::vector<Error>& errors) {
        for (const auto& error : errors) {
            printError(error);
        }
    }
}