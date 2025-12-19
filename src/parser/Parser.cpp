#include "Parser.h"
#include <memory>

Parser::Parser(Lexer& lexer) : lexer(lexer), current(lexer.nextToken()) {
    previous = current;
}

void Parser::advance() {
    previous = current;
    current = lexer.nextToken();
}

bool Parser::check(TokenType type) const {
    return current.type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        advance();
        return true;
    }
    reportError(current, message);
    return false;
}

Token Parser::consume(TokenType type) {
    if (check(type)) {
        Token token = current;
        advance();
        return token;
    }
    reportError(current, "Expected " + std::to_string(static_cast<int>(type)));
    return Token(TokenType::ERROR, "", 0, current.line, current.column);
}

void Parser::reportError(const Token& token, const std::string& message) {
    errors.push_back(Error(ErrorType::SYNTAX, message, token.line, token.column, "Parser"));
}

ProgramPtr Parser::parse() {
    return parseProgram();
}

ProgramPtr Parser::parseProgram() {
    std::vector<StmtPtr> statements;
    
    while (!check(TokenType::END_OF_FILE)) {
        try {
            StmtPtr stmt = parseStatement();
            if (stmt) {
                statements.push_back(stmt);
            }
        } catch (...) {
            // Synchronize on statement boundaries
            while (!check(TokenType::END_OF_FILE) && 
                   !check(TokenType::SEMICOLON) &&
                   !check(TokenType::LET) &&
                   !check(TokenType::IF) &&
                   !check(TokenType::WHILE) &&
                   !check(TokenType::FUNCTION) &&
                   !check(TokenType::PRINT)) {
                advance();
            }
            
            if (check(TokenType::SEMICOLON)) {
                advance();
            }
        }
    }
    
    return std::make_shared<Program>(statements);
}

StmtPtr Parser::parseStatement() {
    if (match(TokenType::LET)) {
        return parseVariableDeclaration();
    } else if (match(TokenType::IF)) {
        return parseIfStatement();
    } else if (match(TokenType::WHILE)) {
        return parseWhileStatement();
    } else if (match(TokenType::FUNCTION)) {
        return parseFunctionDeclaration();
    } else if (match(TokenType::RETURN)) {
        return parseReturnStatement();
    } else if (check(TokenType::IDENTIFIER) && peekNext() == TokenType::LEFT_PAREN) {
        return parseExpressionStatement();
    } else if (check(TokenType::PRINT)) {
        return parsePrintStatement();
    } else {
        return parseExpressionStatement();
    }
}

StmtPtr Parser::parsePrintStatement() {
    Token printToken = previous;
    
    if (!consume(TokenType::LEFT_PAREN, "Expected '(' after 'print'")) {
        return nullptr;
    }
    
    std::vector<ExprPtr> expressions;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            ExprPtr expr = parseExpression();
            if (expr) {
                expressions.push_back(expr);
            }
        } while (match(TokenType::COMMA));
    }
    
    if (!consume(TokenType::RIGHT_PAREN, "Expected ')' after print arguments")) {
        return nullptr;
    }
    
    if (!consume(TokenType::SEMICOLON, "Expected ';' after statement")) {
        return nullptr;
    }
    
    return std::make_shared<PrintStmt>(expressions);
}

StmtPtr Parser::parseVariableDeclaration() {
    if (!consume(TokenType::IDENTIFIER, "Expected variable name")) {
        return nullptr;
    }
    Token name = previous;
    
    if (!match(TokenType::ASSIGN)) {
        reportError(current, "Expected '=' in variable declaration");
        return nullptr;
    }
    
    ExprPtr initializer = parseExpression();
    if (!initializer) {
        reportError(current, "Expected expression in variable declaration");
        return nullptr;
    }
    
    if (!consume(TokenType::SEMICOLON, "Expected ';' after variable declaration")) {
        return nullptr;
    }
    
    return std::make_shared<VariableDeclStmt>(name, initializer);
}

StmtPtr Parser::parseIfStatement() {
    if (!consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'")) {
        return nullptr;
    }
    
    ExprPtr condition = parseExpression();
    if (!condition) {
        reportError(current, "Expected condition expression");
        return nullptr;
    }
    
    if (!consume(TokenType::RIGHT_PAREN, "Expected ')' after condition")) {
        return nullptr;
    }
    
    if (!consume(TokenType::THEN, "Expected 'then' after if condition")) {
        return nullptr;
    }
    
    StmtPtr thenBranch = parseStatement();
    if (!thenBranch) {
        reportError(current, "Expected statement after 'then'");
        return nullptr;
    }
    
    StmtPtr elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        elseBranch = parseStatement();
        if (!elseBranch) {
            reportError(current, "Expected statement after 'else'");
            return nullptr;
        }
    }
    
    if (!consume(TokenType::END, "Expected 'end' after if statement")) {
        return nullptr;
    }
    
    if (!consume(TokenType::SEMICOLON, "Expected ';' after if statement")) {
        return nullptr;
    }
    
    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

StmtPtr Parser::parseWhileStatement() {
    if (!consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'")) {
        return nullptr;
    }
    
    ExprPtr condition = parseExpression();
    if (!condition) {
        reportError(current, "Expected condition expression");
        return nullptr;
    }
    
    if (!consume(TokenType::RIGHT_PAREN, "Expected ')' after condition")) {
        return nullptr;
    }
    
    if (!consume(TokenType::DO, "Expected 'do' after while condition")) {
        return nullptr;
    }
    
    StmtPtr body = parseStatement();
    if (!body) {
        reportError(current, "Expected statement after 'do'");
        return nullptr;
    }
    
    if (!consume(TokenType::END, "Expected 'end' after while statement")) {
        return nullptr;
    }
    
    if (!consume(TokenType::SEMICOLON, "Expected ';' after while statement")) {
        return nullptr;
    }
    
    return std::make_shared<WhileStmt>(condition, body);
}

StmtPtr Parser::parseBlock() {
    if (!consume(TokenType::LEFT_BRACE, "Expected '{' to start block")) {
        return nullptr;
    }
    
    std::vector<StmtPtr> statements;
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        StmtPtr stmt = parseStatement();
        if (stmt) {
            statements.push_back(stmt);
        }
    }
    
    if (!consume(TokenType::RIGHT_BRACE, "Expected '}' to end block")) {
        return nullptr;
    }
    
    return std::make_shared<BlockStmt>(statements);
}

StmtPtr Parser::parseFunctionDeclaration() {
    if (!consume(TokenType::IDENTIFIER, "Expected function name")) {
        return nullptr;
    }
    Token name = previous;
    
    if (!consume(TokenType::LEFT_PAREN, "Expected '(' after function name")) {
        return nullptr;
    }
    
    std::vector<std::pair<Token, TokenType>> parameters;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (!consume(TokenType::IDENTIFIER, "Expected parameter name")) {
                return nullptr;
            }
            Token paramName = previous;
            
            if (!consume(TokenType::COLON, "Expected ':' after parameter name")) {
                return nullptr;
            }
            
            TokenType paramType = TokenType::ERROR;
            if (match(TokenType::INT_TYPE)) paramType = TokenType::INT_TYPE;
            else if (match(TokenType::FLOAT_TYPE)) paramType = TokenType::FLOAT_TYPE;
            else if (match(TokenType::BOOL_TYPE)) paramType = TokenType::BOOL_TYPE;
            else if (match(TokenType::STRING_TYPE)) paramType = TokenType::STRING_TYPE;
            else {
                reportError(current, "Expected type after ':'");
                return nullptr;
            }
            
            parameters.emplace_back(paramName, paramType);
        } while (match(TokenType::COMMA));
    }
    
    if (!consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters")) {
        return nullptr;
    }
    
    TokenType returnType = TokenType::VOID_TYPE;
    if (match(TokenType::COLON)) {
        if (match(TokenType::INT_TYPE)) returnType = TokenType::INT_TYPE;
        else if (match(TokenType::FLOAT_TYPE)) returnType = TokenType::FLOAT_TYPE;
        else if (match(TokenType::BOOL_TYPE)) returnType = TokenType::BOOL_TYPE;
        else if (match(TokenType::STRING_TYPE)) returnType = TokenType::STRING_TYPE;
        else if (match(TokenType::VOID_TYPE)) returnType = TokenType::VOID_TYPE;
        else {
            reportError(current, "Expected return type after ':'");
            return nullptr;
        }
    }
    
    StmtPtr body = parseBlock();
    if (!body) {
        reportError(current, "Expected function body");
        return nullptr;
    }
    
    return std::make_shared<FunctionDeclStmt>(name, parameters, returnType, body);
}

StmtPtr Parser::parseReturnStatement() {
    Token keyword = previous;
    
    ExprPtr value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = parseExpression();
    }
    
    if (!consume(TokenType::SEMICOLON, "Expected ';' after return statement")) {
        return nullptr;
    }
    
    return std::make_shared<ReturnStmt>(keyword, value);
}

StmtPtr Parser::parseExpressionStatement() {
    ExprPtr expr = parseExpression();
    if (!expr) {
        reportError(current, "Expected expression");
        return nullptr;
    }
    
    if (!consume(TokenType::SEMICOLON, "Expected ';' after expression")) {
        return nullptr;
    }
    
    return std::make_shared<ExpressionStmt>(expr);
}

ExprPtr Parser::parseExpression() {
    return parseAssignment();
}

ExprPtr Parser::parseAssignment() {
    ExprPtr expr = parseEquality();
    
    if (match(TokenType::ASSIGN)) {
        if (auto varExpr = std::dynamic_pointer_cast<VariableExpr>(expr)) {
            ExprPtr value = parseAssignment();
            if (value) {
                return std::make_shared<AssignmentExpr>(varExpr->name, value);
            }
        } else {
            reportError(previous, "Invalid assignment target");
        }
    }
    
    return expr;
}

ExprPtr Parser::parseEquality() {
    ExprPtr expr = parseComparison();
    
    while (match(TokenType::EQUAL) || match(TokenType::NOT_EQUAL)) {
        Token op = previous;
        ExprPtr right = parseComparison();
        if (right) {
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }
    }
    
    return expr;
}

ExprPtr Parser::parseComparison() {
    ExprPtr expr = parseTerm();
    
    while (match(TokenType::LESS) || match(TokenType::LESS_EQUAL) || 
           match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL)) {
        Token op = previous;
        ExprPtr right = parseTerm();
        if (right) {
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }
    }
    
    return expr;
}

ExprPtr Parser::parseTerm() {
    ExprPtr expr = parseFactor();
    
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        Token op = previous;
        ExprPtr right = parseFactor();
        if (right) {
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }
    }
    
    return expr;
}

ExprPtr Parser::parseFactor() {
    ExprPtr expr = parseUnary();
    
    while (match(TokenType::MULTIPLY) || match(TokenType::DIVIDE) || match(TokenType::MODULO)) {
        Token op = previous;
        ExprPtr right = parseUnary();
        if (right) {
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }
    }
    
    return expr;
}

ExprPtr Parser::parseUnary() {
    if (match(TokenType::MINUS) || match(TokenType::NOT)) {
        Token op = previous;
        ExprPtr right = parseUnary();
        if (right) {
            return std::make_shared<UnaryExpr>(op, right);
        }
    }
    
    return parseCall();
}

ExprPtr Parser::parseCall() {
    ExprPtr expr = parsePrimary();
    
    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = finishCall(expr);
        } else {
            break;
        }
    }
    
    return expr;
}

ExprPtr Parser::parsePrimary() {
    if (match(TokenType::INT_LITERAL)) {
        return std::make_shared<LiteralExpr>(previous.value);
    }
    if (match(TokenType::FLOAT_LITERAL)) {
        return std::make_shared<LiteralExpr>(previous.value);
    }
    if (match(TokenType::BOOL_LITERAL)) {
        return std::make_shared<LiteralExpr>(previous.value);
    }
    if (match(TokenType::STRING_LITERAL)) {
        return std::make_shared<LiteralExpr>(previous.value);
    }
    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<VariableExpr>(previous);
    }
    if (match(TokenType::LEFT_PAREN)) {
        ExprPtr expr = parseExpression();
        if (!consume(TokenType::RIGHT_PAREN, "Expected ')' after expression")) {
            return nullptr;
        }
        return expr;
    }
    
    reportError(current, "Expected expression");
    return nullptr;
}

ExprPtr Parser::finishCall(ExprPtr callee) {
    std::vector<ExprPtr> arguments;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                reportError(current, "Cannot have more than 255 arguments");
            }
            ExprPtr arg = parseExpression();
            if (arg) {
                arguments.push_back(arg);
            }
        } while (match(TokenType::COMMA));
    }
    
    if (!consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments")) {
        return nullptr;
    }
    
    if (auto varExpr = std::dynamic_pointer_cast<VariableExpr>(callee)) {
        return std::make_shared<CallExpr>(varExpr->name, arguments);
    }
    
    reportError(previous, "Expected function name");
    return callee;
}