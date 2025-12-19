#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <string>
#include <unordered_map>

class Lexer {
private:
    std::string source;
    size_t start;
    size_t current;
    int line;
    int column;
    
    std::unordered_map<std::string, TokenType> keywords;
    
    void initKeywords();
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    void skipWhitespace();
    void skipComment();
    Token makeToken(TokenType type) const;
    Token makeToken(TokenType type, std::variant<int, float, bool, std::string> value) const;
    Token errorToken(const std::string& message) const;
    Token stringLiteral();
    Token numberLiteral();
    Token identifier();
    
public:
    Lexer(const std::string& source);
    Token nextToken();
    int getLine() const { return line; }
    int getColumn() const { return column; }
};

#endif