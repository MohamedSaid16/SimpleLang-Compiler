#include "Lexer.h"
#include <cctype>
#include <sstream>
#include <algorithm>

Lexer::Lexer(const std::string& source) 
    : source(source), start(0), current(0), line(1), column(1) {
    initKeywords();
}

void Lexer::initKeywords() {
    keywords = {
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
        {"false", TokenType::BOOL_LITERAL}
    };
}

bool Lexer::isAtEnd() const {
    return current >= source.length();
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    char c = source[current++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source[current] != expected) return false;
    current++;
    column++;
    return true;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else if (c == '\n') {
            line++;
            column = 1;
            advance();
        } else if (c == '#') {
            skipComment();
        } else {
            break;
        }
    }
}

void Lexer::skipComment() {
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

Token Lexer::makeToken(TokenType type) const {
    std::string text = source.substr(start, current - start);
    return Token(type, text, 0, line, column - text.length());
}

Token Lexer::makeToken(TokenType type, std::variant<int, float, bool, std::string> value) const {
    std::string text = source.substr(start, current - start);
    return Token(type, text, value, line, column - text.length());
}

Token Lexer::errorToken(const std::string& message) const {
    return Token(TokenType::ERROR, message, 0, line, column);
}

Token Lexer::stringLiteral() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
            column = 1;
        }
        advance();
    }
    
    if (isAtEnd()) {
        return errorToken("Unterminated string");
    }
    
    // Skip the closing quote
    advance();
    
    std::string value = source.substr(start + 1, current - start - 2);
    return makeToken(TokenType::STRING_LITERAL, value);
}

Token Lexer::numberLiteral() {
    bool isFloat = false;
    
    while (std::isdigit(peek())) {
        advance();
    }
    
    if (peek() == '.' && std::isdigit(peekNext())) {
        isFloat = true;
        advance(); // Consume the dot
        
        while (std::isdigit(peek())) {
            advance();
        }
    }
    
    std::string text = source.substr(start, current - start);
    
    if (isFloat) {
        float value = std::stof(text);
        return makeToken(TokenType::FLOAT_LITERAL, value);
    } else {
        int value = std::stoi(text);
        return makeToken(TokenType::INT_LITERAL, value);
    }
}

Token Lexer::identifier() {
    while (std::isalnum(peek()) || peek() == '_') {
        advance();
    }
    
    std::string text = source.substr(start, current - start);
    
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        if (text == "true") {
            return makeToken(it->second, true);
        } else if (text == "false") {
            return makeToken(it->second, false);
        }
        return makeToken(it->second);
    }
    
    return makeToken(TokenType::IDENTIFIER);
}

Token Lexer::nextToken() {
    skipWhitespace();
    start = current;
    
    if (isAtEnd()) {
        return makeToken(TokenType::END_OF_FILE);
    }
    
    char c = advance();
    
    if (std::isalpha(c) || c == '_') {
        return identifier();
    }
    
    if (std::isdigit(c)) {
        return numberLiteral();
    }
    
    switch (c) {
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case ',': return makeToken(TokenType::COMMA);
        case ':': return makeToken(TokenType::COLON);
        case ';': return makeToken(TokenType::SEMICOLON);
        
        case '+': return makeToken(TokenType::PLUS);
        case '-': return makeToken(TokenType::MINUS);
        case '*': return makeToken(TokenType::MULTIPLY);
        case '/': return makeToken(TokenType::DIVIDE);
        case '%': return makeToken(TokenType::MODULO);
        
        case '=':
            if (match('=')) return makeToken(TokenType::EQUAL);
            return makeToken(TokenType::ASSIGN);
        
        case '!':
            if (match('=')) return makeToken(TokenType::NOT_EQUAL);
            return makeToken(TokenType::NOT);
        
        case '<':
            if (match('=')) return makeToken(TokenType::LESS_EQUAL);
            return makeToken(TokenType::LESS);
        
        case '>':
            if (match('=')) return makeToken(TokenType::GREATER_EQUAL);
            return makeToken(TokenType::GREATER);
        
        case '&':
            if (match('&')) return makeToken(TokenType::AND);
            break;
        
        case '|':
            if (match('|')) return makeToken(TokenType::OR);
            break;
        
        case '"': return stringLiteral();
    }
    
    return errorToken("Unexpected character: '" + std::string(1, c) + "'");
}