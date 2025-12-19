#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "../lexer/Token.h"
#include <vector>

// Grammar production rules
struct ProductionRule {
    TokenType lhs;  // Non-terminal
    std::vector<std::vector<TokenType>> rhs;  // Multiple possible productions
};

class Grammar {
private:
    static std::vector<ProductionRule> rules;
    
public:
    static void initialize();
    static const std::vector<ProductionRule>& getRules();
    static bool isValidProduction(TokenType nonTerminal, const std::vector<TokenType>& production);
};

#endif