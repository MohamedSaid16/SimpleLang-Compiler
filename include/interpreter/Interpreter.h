#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Environment.h"
#include "../parser/AST.h"
#include "../core/Error.h"
#include <memory>
#include <vector>
#include <functional>

class Interpreter : public Visitor {
private:
    std::shared_ptr<Environment> globalEnv;
    std::shared_ptr<Environment> currentEnv;
    std::vector<Error> errors;
    Value returnValue;
    bool hasReturn;
    
    // Runtime helpers
    Value evaluate(const ExprPtr& expr);
    void execute(const StmtPtr& stmt);
    void executeBlock(const std::vector<StmtPtr>& statements, std::shared_ptr<Environment> env);
    
    // Type conversion helpers
    int toInt(const Value& value);
    float toFloat(const Value& value);
    bool toBool(const Value& value);
    std::string toString(const Value& value);
    
    // Arithmetic operations
    Value add(const Value& left, const Value& right);
    Value subtract(const Value& left, const Value& right);
    Value multiply(const Value& left, const Value& right);
    Value divide(const Value& left, const Value& right);
    Value modulo(const Value& left, const Value& right);
    
    // Comparison operations
    bool equal(const Value& left, const Value& right);
    bool notEqual(const Value& left, const Value& right);
    bool less(const Value& left, const Value& right);
    bool greater(const Value& left, const Value& right);
    bool lessEqual(const Value& left, const Value& right);
    bool greaterEqual(const Value& left, const Value& right);
    
    // Logical operations
    Value logicalAnd(const Value& left, const Value& right);
    Value logicalOr(const Value& left, const Value& right);
    Value logicalNot(const Value& value);
    
    // Error reporting
    void runtimeError(const Token& token, const std::string& message);
    
public:
    Interpreter();
    
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
    
    // Main interpretation method
    void interpret(const ProgramPtr& program);
    const std::vector<Error>& getErrors() const { return errors; }
    bool hasErrors() const { return !errors.empty(); }
    
    // Built-in functions
    void defineNativeFunctions();
};

#endif