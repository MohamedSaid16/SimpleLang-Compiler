#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include <memory>
#include <variant>
#include "../lexer/Token.h"

using RuntimeValue = std::variant<int, float, bool, std::string, nullptr_t>;

struct FunctionObject {
    std::vector<std::pair<std::string, TokenType>> parameters;
    TokenType returnType;
    std::shared_ptr<class BlockStmt> body;
    std::shared_ptr<class Environment> closure;
};

using Value = std::variant<int, float, bool, std::string, nullptr_t, FunctionObject>;

class Environment {
private:
    std::unordered_map<std::string, Value> values;
    std::shared_ptr<Environment> parent;
    
public:
    Environment(std::shared_ptr<Environment> parent = nullptr);
    
    void define(const std::string& name, const Value& value);
    void assign(const std::string& name, const Value& value);
    Value get(const std::string& name);
    bool exists(const std::string& name) const;
    
    std::shared_ptr<Environment> getParent() const { return parent; }
    void setParent(std::shared_ptr<Environment> parent) { this->parent = parent; }
    
    // Helper methods for type checking at runtime
    static bool isTruthy(const Value& value);
    static bool isEqual(const Value& a, const Value& b);
    static std::string valueToString(const Value& value);
    
    void print() const;
};

#endif