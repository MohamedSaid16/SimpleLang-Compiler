#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant>

enum class TokenType {
    // Keywords
    LET, IF, ELSE, WHILE, FOR, FUNCTION, RETURN, END, THEN, DO,
    
    // Types
    INT_TYPE, FLOAT_TYPE, BOOL_TYPE, STRING_TYPE, VOID_TYPE,
    
    // Literals
    INT_LITERAL, FLOAT_LITERAL, BOOL_LITERAL, STRING_LITERAL,
    IDENTIFIER,
    
    // Operators
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
    EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
    ASSIGN, AND, OR, NOT,
    
    // Delimiters
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, COLON, SEMICOLON,
    
    // Special
    END_OF_FILE, ERROR
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::variant<int, float, bool, std::string> value;
    int line;
    int column;
    
    Token(TokenType type = TokenType::ERROR, 
          std::string lexeme = "",
          std::variant<int, float, bool, std::string> value = 0,
          int line = 1, 
          int column = 1);
    
    std::string toString() const;
    std::string typeToString() const;
};

#endif