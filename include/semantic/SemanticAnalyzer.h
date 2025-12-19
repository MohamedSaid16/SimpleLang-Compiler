#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include "SymbolTable.h"
#include "TypeChecker.h"
#include "../parser/AST.h"
#include "../core/Error.h"
#include <memory>
#include <vector>

class SemanticAnalyzer : public Visitor {
private:
    std::shared_ptr<SymbolTable> globalScope;
    std::shared_ptr<SymbolTable> currentScope;
    std::vector<Error> errors;
    TypeChecker typeChecker;
    
    // Analysis helpers
    void enterScope();
    void exitScope();
    void declareVariable(const Token& name, TokenType type, bool initialized = false);
    void defineVariable(const Token& name);
    void declareFunction(const Token& name, TokenType returnType);
    void defineFunction(const Token& name, const std::vector<std::pair<Token, TokenType>>& params);
    
    // Error reporting
    void reportError(const Token& token, const std::string& message);
    
public:
    SemanticAnalyzer();
    
    // Expression visitors
    Value visitLiteralExpr(const LiteralExpr& expr) override;
    Value visitVariableExpr(const VariableExpr& expr) override;
    Value visitBinaryExpr(const BinaryExpr& expr) override;
    Value visitUnaryExpr(const UnaryExpr& expr) override;
    Value visitCallExpr(const CallExpr& expr) override;
    Value visitAssignmentExpr(const AssignmentExpr& expr) override;
    
    // Statement visitors
    void visitPrintStmt(const PrintStmt& stmt) override;
    void visitVariableDeclStmt(const VariableDeclStmt& stmt) override;
    void visitExpressionStmt(const ExpressionStmt& stmt) override;
    void visitBlockStmt(const BlockStmt& stmt) override;
    void visitIfStmt(const IfStmt& stmt) override;
    void visitWhileStmt(const WhileStmt& stmt) override;
    void visitFunctionDeclStmt(const FunctionDeclStmt& stmt) override;
    void visitReturnStmt(const ReturnStmt& stmt) override;
    
    // Main analysis method
    void analyze(const ProgramPtr& program);
    const std::vector<Error>& getErrors() const { return errors; }
    bool hasErrors() const { return !errors.empty(); }
};

#endif