#include "CodeGenerator.h"
#include <iostream>

CodeGenerator::CodeGenerator() : nextVariableIndex(0) {
    // Start with global scope
    scopes.push_back(std::unordered_map<std::string, size_t>());
}

void CodeGenerator::enterScope() {
    scopes.push_back(std::unordered_map<std::string, size_t>());
}

void CodeGenerator::exitScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

size_t CodeGenerator::resolveVariable(const std::string& name) {
    // Search from innermost to outermost scope
    for (int i = scopes.size() - 1; i >= 0; i--) {
        auto it = scopes[i].find(name);
        if (it != scopes[i].end()) {
            return it->second;
        }
    }
    
    // Variable not found - this should have been caught by semantic analysis
    return -1;
}

void CodeGenerator::declareVariable(const std::string& name) {
    if (!scopes.empty()) {
        scopes.back()[name] = nextVariableIndex++;
    }
}

// Expression visitors
Value CodeGenerator::visitLiteralExpr(const LiteralExpr& expr) {
    size_t constIndex = writer.addConstantGetIndex(expr.value);
    writer.writeOpCode(OpCode::LOAD_CONST);
    writer.writeOperand(static_cast<uint32_t>(constIndex));
    return nullptr;
}

Value CodeGenerator::visitVariableExpr(const VariableExpr& expr) {
    size_t varIndex = resolveVariable(expr.name.lexeme);
    if (varIndex != static_cast<size_t>(-1)) {
        writer.writeOpCode(OpCode::LOAD_VAR);
        writer.writeOperand(static_cast<uint32_t>(varIndex));
    }
    return nullptr;
}

Value CodeGenerator::visitBinaryExpr(const BinaryExpr& expr) {
    // Generate code for left operand
    expr.left->accept(*this);
    
    // Generate code for right operand
    expr.right->accept(*this);
    
    // Generate operation code
    switch (expr.op.type) {
        case TokenType::PLUS:
            writer.writeOpCode(OpCode::ADD);
            break;
        case TokenType::MINUS:
            writer.writeOpCode(OpCode::SUB);
            break;
        case TokenType::MULTIPLY:
            writer.writeOpCode(OpCode::MUL);
            break;
        case TokenType::DIVIDE:
            writer.writeOpCode(OpCode::DIV);
            break;
        case TokenType::MODULO:
            writer.writeOpCode(OpCode::MOD);
            break;
        case TokenType::EQUAL:
            writer.writeOpCode(OpCode::EQ);
            break;
        case TokenType::NOT_EQUAL:
            writer.writeOpCode(OpCode::NEQ);
            break;
        case TokenType::LESS:
            writer.writeOpCode(OpCode::LT);
            break;
        case TokenType::GREATER:
            writer.writeOpCode(OpCode::GT);
            break;
        case TokenType::LESS_EQUAL:
            writer.writeOpCode(OpCode::LTE);
            break;
        case TokenType::GREATER_EQUAL:
            writer.writeOpCode(OpCode::GTE);
            break;
        case TokenType::AND:
            writer.writeOpCode(OpCode::AND);
            break;
        case TokenType::OR:
            writer.writeOpCode(OpCode::OR);
            break;
        default:
            // Unknown operator
            break;
    }
    
    return nullptr;
}

Value CodeGenerator::visitUnaryExpr(const UnaryExpr& expr) {
    // Generate code for operand
    expr.right->accept(*this);
    
    // Generate operation code
    switch (expr.op.type) {
        case TokenType::MINUS:
            writer.writeOpCode(OpCode::NEG);
            break;
        case TokenType::NOT:
            writer.writeOpCode(OpCode::NOT);
            break;
        default:
            // Unknown operator
            break;
    }
    
    return nullptr;
}

Value CodeGenerator::visitCallExpr(const CallExpr& expr) {
    // Generate code for each argument
    for (auto& arg : expr.arguments) {
        arg->accept(*this);
    }
    
    // Generate call instruction
    // For now, we'll assume it's a built-in function
    if (expr.callee.lexeme == "print") {
        writer.writeOpCode(OpCode::PRINT);
    } else {
        writer.writeOpCode(OpCode::CALL);
        // Would need function index
    }
    
    return nullptr;
}

Value CodeGenerator::visitAssignmentExpr(const AssignmentExpr& expr) {
    // Generate code for value
    expr.value->accept(*this);
    
    // Generate store instruction
    size_t varIndex = resolveVariable(expr.name.lexeme);
    if (varIndex != static_cast<size_t>(-1)) {
        writer.writeOpCode(OpCode::STORE_VAR);
        writer.writeOperand(static_cast<uint32_t>(varIndex));
    }
    
    // Leave value on stack
    varIndex = resolveVariable(expr.name.lexeme);
    if (varIndex != static_cast<size_t>(-1)) {
        writer.writeOpCode(OpCode::LOAD_VAR);
        writer.writeOperand(static_cast<uint32_t>(varIndex));
    }
    
    return nullptr;
}

// Statement visitors
void CodeGenerator::visitPrintStmt(const PrintStmt& stmt) {
    for (auto& expr : stmt.expressions) {
        expr->accept(*this);
    }
    writer.writeOpCode(OpCode::PRINT);
}

void CodeGenerator::visitVariableDeclStmt(const VariableDeclStmt& stmt) {
    // Declare variable
    declareVariable(stmt.name.lexeme);
    
    if (stmt.initializer) {
        // Generate code for initializer
        stmt.initializer->accept(*this);
        
        // Store initial value
        size_t varIndex = resolveVariable(stmt.name.lexeme);
        writer.writeOpCode(OpCode::STORE_VAR);
        writer.writeOperand(static_cast<uint32_t>(varIndex));
    } else {
        // Initialize with null
        writer.writeOpCode(OpCode::LOAD_NULL);
        
        size_t varIndex = resolveVariable(stmt.name.lexeme);
        writer.writeOpCode(OpCode::STORE_VAR);
        writer.writeOperand(static_cast<uint32_t>(varIndex));
    }
}

void CodeGenerator::visitExpressionStmt(const ExpressionStmt& stmt) {
    if (stmt.expression) {
        stmt.expression->accept(*this);
        // Pop the result if not used
        writer.writeOpCode(OpCode::POP);
    }
}

void CodeGenerator::visitBlockStmt(const BlockStmt& stmt) {
    enterScope();
    
    for (auto& stmtPtr : stmt.statements) {
        stmtPtr->accept(*this);
    }
    
    exitScope();
}

void CodeGenerator::visitIfStmt(const IfStmt& stmt) {
    // Generate code for condition
    stmt.condition->accept(*this);
    
    // Remember position for jump
    size_t jumpIfFalsePos = writer.getCode().size();
    writer.writeOpCode(OpCode::JUMP_IF_FALSE);
    writer.writeOperand(0); // Placeholder - will be patched
    
    // Generate code for then branch
    stmt.thenBranch->accept(*this);
    
    size_t jumpPos = writer.getCode().size();
    writer.writeOpCode(OpCode::JUMP);
    writer.writeOperand(0); // Placeholder
    
    // Patch jump_if_false
    // This would require modifying the bytecode after generation
    // For simplicity, we'll leave it as is for now
    
    if (stmt.elseBranch) {
        // Generate code for else branch
        stmt.elseBranch->accept(*this);
    }
    
    // Patch jumps
    // Implementation would go here
}

void CodeGenerator::visitWhileStmt(const WhileStmt& stmt) {
    size_t loopStart = writer.getCode().size();
    
    // Generate code for condition
    stmt.condition->accept(*this);
    
    size_t jumpIfFalsePos = writer.getCode().size();
    writer.writeOpCode(OpCode::JUMP_IF_FALSE);
    writer.writeOperand(0); // Placeholder
    
    // Generate code for body
    stmt.body->accept(*this);
    
    // Jump back to condition
    writer.writeOpCode(OpCode::JUMP);
    writer.writeOperand(static_cast<uint32_t>(loopStart));
    
    // Patch jump_if_false
    // Implementation would go here
}

void CodeGenerator::visitFunctionDeclStmt(const FunctionDeclStmt& stmt) {
    // Function declaration would generate a function object
    // For now, we'll skip code generation for functions
    // In a real compiler, this would create a new code segment
}

void CodeGenerator::visitReturnStmt(const ReturnStmt& stmt) {
    if (stmt.value) {
        stmt.value->accept(*this);
    } else {
        writer.writeOpCode(OpCode::LOAD_NULL);
    }
    writer.writeOpCode(OpCode::RETURN);
}

BytecodeWriter CodeGenerator::generate(const ProgramPtr& program) {
    for (auto& stmt : program->statements) {
        stmt->accept(*this);
    }
    
    // Add halt instruction at the end
    writer.writeOpCode(OpCode::HALT);
    
    return writer;
}