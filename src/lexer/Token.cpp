#include "Token.h"
#include <sstream>

Token::Token(TokenType type, std::string lexeme, 
             std::variant<int, float, bool, std::string> value,
             int line, int column)
    : type(type), lexeme(lexeme), value(value), line(line), column(column) {}

std::string Token::toString() const {
    std::stringstream ss;
    ss << typeToString() << " '" << lexeme << "'";
    
    // Add value if present
    if (std::holds_alternative<int>(value)) {
        ss << " (value: " << std::get<int>(value) << ")";
    } else if (std::holds_alternative<float>(value)) {
        ss << " (value: " << std::get<float>(value) << ")";
    } else if (std::holds_alternative<bool>(value)) {
        ss << " (value: " << (std::get<bool>(value) ? "true" : "false") << ")";
    } else if (std::holds_alternative<std::string>(value)) {
        ss << " (value: \"" << std::get<std::string>(value) << "\")";
    }
    
    ss << " at " << line << ":" << column;
    return ss.str();
}

std::string Token::typeToString() const {
    switch (type) {
        // Keywords
        case TokenType::LET: return "LET";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::FOR: return "FOR";
        case TokenType::FUNCTION: return "FUNCTION";
        case TokenType::RETURN: return "RETURN";
        case TokenType::END: return "END";
        case TokenType::THEN: return "THEN";
        case TokenType::DO: return "DO";
        
        // Types
        case TokenType::INT_TYPE: return "INT_TYPE";
        case TokenType::FLOAT_TYPE: return "FLOAT_TYPE";
        case TokenType::BOOL_TYPE: return "BOOL_TYPE";
        case TokenType::STRING_TYPE: return "STRING_TYPE";
        case TokenType::VOID_TYPE: return "VOID_TYPE";
        
        // Literals
        case TokenType::INT_LITERAL: return "INT_LITERAL";
        case TokenType::FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TokenType::BOOL_LITERAL: return "BOOL_LITERAL";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        
        // Operators
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::MODULO: return "MODULO";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        
        // Delimiters
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::COLON: return "COLON";
        case TokenType::SEMICOLON: return "SEMICOLON";
        
        // Special
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::ERROR: return "ERROR";
        
        default: return "UNKNOWN";
    }
}