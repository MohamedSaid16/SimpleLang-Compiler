#include "Keywords.h"

std::unordered_map<std::string, TokenType> Keywords::keywordMap = {
    {"let", TokenType::LET},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"function", TokenType::FUNCTION},
    {"return", TokenType::RETURN},
    {"end", TokenType::END},
    {"then", TokenType::THEN},
    {"do", TokenType::DO},
    {"int", TokenType::INT_TYPE},
    {"float", TokenType::FLOAT_TYPE},
    {"bool", TokenType::BOOL_TYPE},
    {"string", TokenType::STRING_TYPE},
    {"void", TokenType::VOID_TYPE},
    {"true", TokenType::BOOL_LITERAL},
    {"false", TokenType::BOOL_LITERAL},
    {"print", TokenType::PRINT},
    {"input", TokenType::INPUT}
};

void Keywords::initialize() {
    // Already initialized statically
}

TokenType Keywords::getKeyword(const std::string& word) {
    auto it = keywordMap.find(word);
    if (it != keywordMap.end()) {
        return it->second;
    }
    return TokenType::IDENTIFIER;
}

bool Keywords::isKeyword(const std::string& word) {
    return keywordMap.find(word) != keywordMap.end();
}