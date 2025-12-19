#include "Grammar.h"
#include <algorithm>

std::vector<ProductionRule> Grammar::rules = {
    // Program rules
    {TokenType::PROGRAM, {{TokenType::STATEMENT_LIST}}},
    
    // Statement rules
    {TokenType::STATEMENT_LIST, {
        {TokenType::STATEMENT, TokenType::STATEMENT_LIST},
        {}  // Empty production
    }},
    
    {TokenType::STATEMENT, {
        {TokenType::EXPRESSION_STMT},
        {TokenType::VARIABLE_DECL},
        {TokenType::IF_STMT},
        {TokenType::WHILE_STMT},
        {TokenType::FUNCTION_DECL},
        {TokenType::RETURN_STMT},
        {TokenType::PRINT_STMT}
    }},
    
    // Expression rules
    {TokenType::EXPRESSION, {
        {TokenType::ASSIGNMENT_EXPR}
    }},
    
    {TokenType::ASSIGNMENT_EXPR, {
        {TokenType::LOGIC_OR_EXPR, TokenType::ASSIGN, TokenType::ASSIGNMENT_EXPR},
        {TokenType::LOGIC_OR_EXPR}
    }},
    
    {TokenType::LOGIC_OR_EXPR, {
        {TokenType::LOGIC_AND_EXPR, TokenType::OR, TokenType::LOGIC_OR_EXPR},
        {TokenType::LOGIC_AND_EXPR}
    }},
    
    {TokenType::LOGIC_AND_EXPR, {
        {TokenType::EQUALITY_EXPR, TokenType::AND, TokenType::LOGIC_AND_EXPR},
        {TokenType::EQUALITY_EXPR}
    }},
    
    {TokenType::EQUALITY_EXPR, {
        {TokenType::COMPARISON_EXPR, TokenType::EQUALITY_OP, TokenType::EQUALITY_EXPR},
        {TokenType::COMPARISON_EXPR}
    }},
    
    {TokenType::COMPARISON_EXPR, {
        {TokenType::TERM, TokenType::COMPARISON_OP, TokenType::COMPARISON_EXPR},
        {TokenType::TERM}
    }},
    
    {TokenType::TERM, {
        {TokenType::TERM, TokenType::ADD_OP, TokenType::FACTOR},
        {TokenType::FACTOR}
    }},
    
    {TokenType::FACTOR, {
        {TokenType::FACTOR, TokenType::MUL_OP, TokenType::UNARY},
        {TokenType::UNARY}
    }},
    
    {TokenType::UNARY, {
        {TokenType::UNARY_OP, TokenType::UNARY},
        {TokenType::CALL}
    }},
    
    {TokenType::CALL, {
        {TokenType::PRIMARY, TokenType::LEFT_PAREN, TokenType::ARGUMENTS, TokenType::RIGHT_PAREN},
        {TokenType::PRIMARY}
    }},
    
    {TokenType::ARGUMENTS, {
        {TokenType::EXPRESSION, TokenType::ARGUMENT_LIST},
        {}  // Empty
    }},
    
    {TokenType::ARGUMENT_LIST, {
        {TokenType::COMMA, TokenType::EXPRESSION, TokenType::ARGUMENT_LIST},
        {}  // Empty
    }},
    
    {TokenType::PRIMARY, {
        {TokenType::INT_LITERAL},
        {TokenType::FLOAT_LITERAL},
        {TokenType::BOOL_LITERAL},
        {TokenType::STRING_LITERAL},
        {TokenType::IDENTIFIER},
        {TokenType::LEFT_PAREN, TokenType::EXPRESSION, TokenType::RIGHT_PAREN}
    }}
};

void Grammar::initialize() {
    // Already initialized statically
}

const std::vector<ProductionRule>& Grammar::getRules() {
    return rules;
}

bool Grammar::isValidProduction(TokenType nonTerminal, const std::vector<TokenType>& production) {
    for (const auto& rule : rules) {
        if (rule.lhs == nonTerminal) {
            for (const auto& prod : rule.rhs) {
                if (prod == production) {
                    return true;
                }
            }
        }
    }
    return false;
}