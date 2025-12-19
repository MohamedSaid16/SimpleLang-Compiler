#include "AST.h"

// Expression accept methods
Value LiteralExpr::accept(Visitor& visitor) const {
    return visitor.visitLiteralExpr(*this);
}

Value VariableExpr::accept(Visitor& visitor) const {
    return visitor.visitVariableExpr(*this);
}

Value BinaryExpr::accept(Visitor& visitor) const {
    return visitor.visitBinaryExpr(*this);
}

Value UnaryExpr::accept(Visitor& visitor) const {
    return visitor.visitUnaryExpr(*this);
}

Value CallExpr::accept(Visitor& visitor) const {
    return visitor.visitCallExpr(*this);
}

Value AssignmentExpr::accept(Visitor& visitor) const {
    return visitor.visitAssignmentExpr(*this);
}

// Statement accept methods
void PrintStmt::accept(Visitor& visitor) const {
    visitor.visitPrintStmt(*this);
}

void VariableDeclStmt::accept(Visitor& visitor) const {
    visitor.visitVariableDeclStmt(*this);
}

void ExpressionStmt::accept(Visitor& visitor) const {
    visitor.visitExpressionStmt(*this);
}

void BlockStmt::accept(Visitor& visitor) const {
    visitor.visitBlockStmt(*this);
}

void IfStmt::accept(Visitor& visitor) const {
    visitor.visitIfStmt(*this);
}

void WhileStmt::accept(Visitor& visitor) const {
    visitor.visitWhileStmt(*this);
}

void FunctionDeclStmt::accept(Visitor& visitor) const {
    visitor.visitFunctionDeclStmt(*this);
}

void ReturnStmt::accept(Visitor& visitor) const {
    visitor.visitReturnStmt(*this);
}