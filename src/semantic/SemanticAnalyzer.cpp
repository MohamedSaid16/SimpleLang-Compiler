#include "SemanticAnalyzer.h"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer() {
    globalScope = std::make_shared<SymbolTable>();
    currentScope = globalScope;
}

void SemanticAnalyzer::enterScope() {
    currentScope = std::make_shared<SymbolTable>(currentScope->getScopeLevel() + 1, currentScope);
}

void SemanticAnalyzer::exitScope() {
    if (currentScope->getParent()) {
        currentScope = currentScope->getParent();
    }
}

void SemanticAnalyzer::declareVariable(const Token& name, TokenType type, bool initialized) {
    auto symbol = std::make_shared<Symbol>(name.lexeme, SymbolType::VARIABLE, 
                                          type, currentScope->getScopeLevel(), initialized);
    if (!currentScope->insert(symbol)) {
        reportError(name, "Variable '" + name.lexeme + "' already declared in this scope");
    }
}

void SemanticAnalyzer::defineVariable(const Token& name) {
    auto symbol = currentScope->lookup(name.lexeme);
    if (symbol) {
        symbol->isInitialized = true;
    }
}

void SemanticAnalyzer::reportError(const Token& token, const std::string& message) {
    errors.push_back(Error(ErrorType::SEMANTIC, message, token.line, token.column, "SemanticAnalyzer"));
}

// Expression visitors
Value SemanticAnalyzer::visitLiteralExpr(const LiteralExpr& expr) {
    return nullptr;
}

Value SemanticAnalyzer::visitVariableExpr(const VariableExpr& expr) {
    auto symbol = currentScope->lookup(expr.name.lexeme);
    if (!symbol) {
        reportError(expr.name, "Undefined variable '" + expr.name.lexeme + "'");
    } else if (!symbol->isInitialized) {
        reportError(expr.name, "Variable '" + expr.name.lexeme + "' used before initialization");
    }
    return nullptr;
}

Value SemanticAnalyzer::visitBinaryExpr(const BinaryExpr& expr) {
    expr.left->accept(*this);
    expr.right->accept(*this);
    return nullptr;
}

Value SemanticAnalyzer::visitUnaryExpr(const UnaryExpr& expr) {
    expr.right->accept(*this);
    return nullptr;
}

Value SemanticAnalyzer::visitCallExpr(const CallExpr& expr) {
    auto symbol = currentScope->lookup(expr.callee.lexeme);
    if (!symbol || symbol->type != SymbolType::FUNCTION) {
        reportError(expr.callee, "Undefined function '" + expr.callee.lexeme + "'");
    }
    
    for (auto& arg : expr.arguments) {
        arg->accept(*this);
    }
    return nullptr;
}

Value SemanticAnalyzer::visitAssignmentExpr(const AssignmentExpr& expr) {
    auto symbol = currentScope->lookup(expr.name.lexeme);
    if (!symbol) {
        reportError(expr.name, "Cannot assign to undefined variable '" + expr.name.lexeme + "'");
    } else if (symbol->isConstant) {
        reportError(expr.name, "Cannot assign to constant '" + expr.name.lexeme + "'");
    }
    
    expr.value->accept(*this);
    return nullptr;
}

// Statement visitors
void SemanticAnalyzer::visitPrintStmt(const PrintStmt& stmt) {
    for (auto& expr : stmt.expressions) {
        expr->accept(*this);
    }
}

void SemanticAnalyzer::visitVariableDeclStmt(const VariableDeclStmt& stmt) {
    declareVariable(stmt.name, TokenType::INT_TYPE, stmt.initializer != nullptr);
    
    if (stmt.initializer) {
        stmt.initializer->accept(*this);
        defineVariable(stmt.name);
    }
}

void SemanticAnalyzer::visitExpressionStmt(const ExpressionStmt& stmt) {
    if (stmt.expression) {
        stmt.expression->accept(*this);
    }
}

void SemanticAnalyzer::visitBlockStmt(const BlockStmt& stmt) {
    enterScope();
    
    for (auto& stmtPtr : stmt.statements) {
        stmtPtr->accept(*this);
    }
    
    exitScope();
}

void SemanticAnalyzer::visitIfStmt(const IfStmt& stmt) {
    if (stmt.condition) {
        stmt.condition->accept(*this);
    }
    if (stmt.thenBranch) {
        stmt.thenBranch->accept(*this);
    }
    if (stmt.elseBranch) {
        stmt.elseBranch->accept(*this);
    }
}

void SemanticAnalyzer::visitWhileStmt(const WhileStmt& stmt) {
    if (stmt.condition) {
        stmt.condition->accept(*this);
    }
    if (stmt.body) {
        stmt.body->accept(*this);
    }
}

void SemanticAnalyzer::visitFunctionDeclStmt(const FunctionDeclStmt& stmt) {
    declareVariable(stmt.name, stmt.returnType, true);
    
    enterScope();
    
    // Add parameters to scope
    for (auto& [paramName, paramType] : stmt.parameters) {
        declareVariable(paramName, paramType, true);
    }
    
    if (stmt.body) {
        stmt.body->accept(*this);
    }
    
    exitScope();
}

void SemanticAnalyzer::visitReturnStmt(const ReturnStmt& stmt) {
    if (stmt.value) {
        stmt.value->accept(*this);
    }
}

void SemanticAnalyzer::analyze(const ProgramPtr& program) {
    // First pass: collect declarations
    for (auto& stmt : program->statements) {
        if (auto funcDecl = std::dynamic_pointer_cast<FunctionDeclStmt>(stmt)) {
            declareVariable(funcDecl->name, funcDecl->returnType, true);
        }
    }
    
    // Second pass: analyze statements
    for (auto& stmt : program->statements) {
        stmt->accept(*this);
    }
    
    // Run type checker
    typeChecker.check(program);
    auto typeErrors = typeChecker.getErrors();
    errors.insert(errors.end(), typeErrors.begin(), typeErrors.end());
}