#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include "../lexer/Token.h"

using Value = std::variant<int, float, bool, std::string>;

// Forward declarations
class Expr;
class Stmt;
class Program;

using ExprPtr = std::shared_ptr<Expr>;
using StmtPtr = std::shared_ptr<Stmt>;
using ProgramPtr = std::shared_ptr<Program>;

// Expression types
enum class ExprType {
    LITERAL,
    VARIABLE,
    BINARY,
    UNARY,
    CALL,
    ASSIGNMENT
};

// Statement types
enum class StmtType {
    EXPRESSION,
    PRINT,
    VARIABLE_DECL,
    BLOCK,
    IF,
    WHILE,
    FUNCTION_DECL,
    RETURN
};

// Base Expression class
class Expr {
public:
    virtual ~Expr() = default;
    virtual ExprType getType() const = 0;
    virtual Value accept(class Visitor& visitor) const = 0;
};

// Base Statement class
class Stmt {
public:
    virtual ~Stmt() = default;
    virtual StmtType getType() const = 0;
    virtual void accept(class Visitor& visitor) const = 0;
};

// Literal expression
class LiteralExpr : public Expr {
public:
    Value value;
    
    LiteralExpr(Value value) : value(value) {}
    ExprType getType() const override { return ExprType::LITERAL; }
    Value accept(class Visitor& visitor) const override;
};

// Variable expression
class VariableExpr : public Expr {
public:
    Token name;
    
    VariableExpr(const Token& name) : name(name) {}
    ExprType getType() const override { return ExprType::VARIABLE; }
    Value accept(class Visitor& visitor) const override;
};

// Binary expression
class BinaryExpr : public Expr {
public:
    ExprPtr left;
    Token op;
    ExprPtr right;
    
    BinaryExpr(ExprPtr left, const Token& op, ExprPtr right)
        : left(left), op(op), right(right) {}
    ExprType getType() const override { return ExprType::BINARY; }
    Value accept(class Visitor& visitor) const override;
};

// Unary expression
class UnaryExpr : public Expr {
public:
    Token op;
    ExprPtr right;
    
    UnaryExpr(const Token& op, ExprPtr right)
        : op(op), right(right) {}
    ExprType getType() const override { return ExprType::UNARY; }
    Value accept(class Visitor& visitor) const override;
};

// Function call expression
class CallExpr : public Expr {
public:
    Token callee;
    std::vector<ExprPtr> arguments;
    
    CallExpr(const Token& callee, std::vector<ExprPtr> arguments)
        : callee(callee), arguments(arguments) {}
    ExprType getType() const override { return ExprType::CALL; }
    Value accept(class Visitor& visitor) const override;
};

// Assignment expression
class AssignmentExpr : public Expr {
public:
    Token name;
    ExprPtr value;
    
    AssignmentExpr(const Token& name, ExprPtr value)
        : name(name), value(value) {}
    ExprType getType() const override { return ExprType::ASSIGNMENT; }
    Value accept(class Visitor& visitor) const override;
};

// Print statement
class PrintStmt : public Stmt {
public:
    std::vector<ExprPtr> expressions;
    
    PrintStmt(std::vector<ExprPtr> expressions) : expressions(expressions) {}
    StmtType getType() const override { return StmtType::PRINT; }
    void accept(class Visitor& visitor) const override;
};

// Variable declaration statement
class VariableDeclStmt : public Stmt {
public:
    Token name;
    ExprPtr initializer;
    
    VariableDeclStmt(const Token& name, ExprPtr initializer)
        : name(name), initializer(initializer) {}
    StmtType getType() const override { return StmtType::VARIABLE_DECL; }
    void accept(class Visitor& visitor) const override;
};

// Expression statement
class ExpressionStmt : public Stmt {
public:
    ExprPtr expression;
    
    ExpressionStmt(ExprPtr expression) : expression(expression) {}
    StmtType getType() const override { return StmtType::EXPRESSION; }
    void accept(class Visitor& visitor) const override;
};

// Block statement
class BlockStmt : public Stmt {
public:
    std::vector<StmtPtr> statements;
    
    BlockStmt(std::vector<StmtPtr> statements) : statements(statements) {}
    StmtType getType() const override { return StmtType::BLOCK; }
    void accept(class Visitor& visitor) const override;
};

// If statement
class IfStmt : public Stmt {
public:
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;
    
    IfStmt(ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch = nullptr)
        : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
    StmtType getType() const override { return StmtType::IF; }
    void accept(class Visitor& visitor) const override;
};

// While statement
class WhileStmt : public Stmt {
public:
    ExprPtr condition;
    StmtPtr body;
    
    WhileStmt(ExprPtr condition, StmtPtr body)
        : condition(condition), body(body) {}
    StmtType getType() const override { return StmtType::WHILE; }
    void accept(class Visitor& visitor) const override;
};

// Function declaration
class FunctionDeclStmt : public Stmt {
public:
    Token name;
    std::vector<std::pair<Token, TokenType>> parameters;
    TokenType returnType;
    StmtPtr body;
    
    FunctionDeclStmt(const Token& name, 
                    std::vector<std::pair<Token, TokenType>> parameters,
                    TokenType returnType,
                    StmtPtr body)
        : name(name), parameters(parameters), returnType(returnType), body(body) {}
    StmtType getType() const override { return StmtType::FUNCTION_DECL; }
    void accept(class Visitor& visitor) const override;
};

// Return statement
class ReturnStmt : public Stmt {
public:
    Token keyword;
    ExprPtr value;
    
    ReturnStmt(const Token& keyword, ExprPtr value = nullptr)
        : keyword(keyword), value(value) {}
    StmtType getType() const override { return StmtType::RETURN; }
    void accept(class Visitor& visitor) const override;
};

// Program node
class Program {
public:
    std::vector<StmtPtr> statements;
    
    Program(std::vector<StmtPtr> statements) : statements(statements) {}
};

// Visitor interface
class Visitor {
public:
    virtual ~Visitor() = default;
    
    // Expressions
    virtual Value visitLiteralExpr(const LiteralExpr& expr) = 0;
    virtual Value visitVariableExpr(const VariableExpr& expr) = 0;
    virtual Value visitBinaryExpr(const BinaryExpr& expr) = 0;
    virtual Value visitUnaryExpr(const UnaryExpr& expr) = 0;
    virtual Value visitCallExpr(const CallExpr& expr) = 0;
    virtual Value visitAssignmentExpr(const AssignmentExpr& expr) = 0;
    
    // Statements
    virtual void visitPrintStmt(const PrintStmt& stmt) = 0;
    virtual void visitVariableDeclStmt(const VariableDeclStmt& stmt) = 0;
    virtual void visitExpressionStmt(const ExpressionStmt& stmt) = 0;
    virtual void visitBlockStmt(const BlockStmt& stmt) = 0;
    virtual void visitIfStmt(const IfStmt& stmt) = 0;
    virtual void visitWhileStmt(const WhileStmt& stmt) = 0;
    virtual void visitFunctionDeclStmt(const FunctionDeclStmt& stmt) = 0;
    virtual void visitReturnStmt(const ReturnStmt& stmt) = 0;
};

#endif