#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "SymbolTable.h"
#include "../parser/AST.h"
#include "../core/Error.h"
#include <memory>
#include <vector>

class TypeChecker : public Visitor {
private:
    std::shared_ptr<SymbolTable> currentScope;
    std::vector<Error> errors;
    TokenType currentReturnType;
    bool inFunction;
    
    // Type checking helpers
    TokenType getExpressionType(const ExprPtr& expr);
    bool checkType(TokenType expected, TokenType actual, const Token& token);
    bool isNumericType(TokenType type);
    bool isBooleanType(TokenType type);
    bool isStringType(TokenType type);
    bool typesCompatible(TokenType t1, TokenType t2);
    TokenType getBinaryResultType(TokenType left, TokenType right, TokenType op);
    
    // Error reporting
    void reportError(const Token& token, const std::string& message);
    
public:
    TypeChecker();
    
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
    
    // Main type checking method
    void check(const ProgramPtr& program);
    const std::vector<Error>& getErrors() const { return errors; }
    bool hasErrors() const { return !errors.empty(); }
};

#endif