#ifndef KEYWORDS_H
#define KEYWORDS_H

#include <string>
#include <unordered_map>
#include "Token.h"

class Keywords {
private:
    static std::unordered_map<std::string, TokenType> keywordMap;
    
public:
    static void initialize();
    static TokenType getKeyword(const std::string& word);
    static bool isKeyword(const std::string& word);
};

#endif