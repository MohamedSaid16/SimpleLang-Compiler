#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "Bytecode.h"
#include "../parser/AST.h"
#include <memory>
#include <vector>

class CodeGenerator : public Visitor {
private:
    BytecodeWriter writer;
    std::unordered_map<std::string, size_t> variableIndices;
    std::vector<std::unordered_map<std::string, size_t>> scopes;
    size_t nextVariableIndex;
    
    // Generation helpers
    void enterScope();
    void exitScope();
    size_t resolveVariable(const std::string& name);
    void declareVariable(const std::string& name);
    
    // Control flow
    std::vector<size_t> breakPositions;
    std::vector<size_t> continuePositions;
    
public:
    CodeGenerator();
    
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
    
    // Main generation method
    BytecodeWriter generate(const ProgramPtr& program);
};

#endif