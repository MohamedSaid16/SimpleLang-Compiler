#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "../lexer/Token.h"

enum class SymbolType {
    VARIABLE,
    FUNCTION,
    PARAMETER
};

struct Symbol {
    std::string name;
    SymbolType type;
    TokenType dataType;
    int scopeLevel;
    bool isInitialized;
    bool isConstant;
    
    Symbol(const std::string& name, SymbolType symType, TokenType dataType, 
           int scopeLevel, bool initialized = false, bool constant = false)
        : name(name), type(symType), dataType(dataType),
          scopeLevel(scopeLevel), isInitialized(initialized), isConstant(constant) {}
};

class SymbolTable {
private:
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols;
    std::shared_ptr<SymbolTable> parent;
    int scopeLevel;
    
public:
    SymbolTable(int scopeLevel = 0, std::shared_ptr<SymbolTable> parent = nullptr);
    
    bool insert(std::shared_ptr<Symbol> symbol);
    std::shared_ptr<Symbol> lookup(const std::string& name);
    std::shared_ptr<Symbol> lookupCurrentScope(const std::string& name);
    bool remove(const std::string& name);
    
    std::shared_ptr<SymbolTable> getParent() const { return parent; }
    int getScopeLevel() const { return scopeLevel; }
    void setParent(std::shared_ptr<SymbolTable> parent) { this->parent = parent; }
    
    void print() const;
};

#endif