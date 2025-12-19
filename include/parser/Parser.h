#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "../lexer/Lexer.h"
#include "../core/Error.h"
#include <vector>
#include <memory>

class Parser {
private:
    Lexer& lexer;
    Token current;
    Token previous;
    std::vector<Error> errors;
    
    void advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool consume(TokenType type, const std::string& message);
    Token consume(TokenType type);
    
    // Parsing methods
    ProgramPtr parseProgram();
    StmtPtr parseStatement();
    StmtPtr parsePrintStatement();
    StmtPtr parseVariableDeclaration();
    StmtPtr parseIfStatement();
    StmtPtr parseWhileStatement();
    StmtPtr parseBlock();
    StmtPtr parseFunctionDeclaration();
    StmtPtr parseReturnStatement();
    StmtPtr parseExpressionStatement();
    
    ExprPtr parseExpression();
    ExprPtr parseAssignment();
    ExprPtr parseEquality();
    ExprPtr parseComparison();
    ExprPtr parseTerm();
    ExprPtr parseFactor();
    ExprPtr parseUnary();
    ExprPtr parseCall();
    ExprPtr parsePrimary();
    
    ExprPtr finishCall(ExprPtr callee);
    
public:
    Parser(Lexer& lexer);
    ProgramPtr parse();
    const std::vector<Error>& getErrors() const { return errors; }
    bool hasErrors() const { return !errors.empty(); }
    void reportError(const Token& token, const std::string& message);
};

#endif