#include "SymbolTable.h"
#include <iostream>

SymbolTable::SymbolTable(int scopeLevel, std::shared_ptr<SymbolTable> parent)
    : parent(parent), scopeLevel(scopeLevel) {}

bool SymbolTable::insert(std::shared_ptr<Symbol> symbol) {
    if (symbols.find(symbol->name) != symbols.end()) {
        return false; // Symbol already declared in this scope
    }
    symbols[symbol->name] = symbol;
    return true;
}

std::shared_ptr<Symbol> SymbolTable::lookup(const std::string& name) {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return it->second;
    }
    
    if (parent) {
        return parent->lookup(name);
    }
    
    return nullptr;
}

std::shared_ptr<Symbol> SymbolTable::lookupCurrentScope(const std::string& name) {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return it->second;
    }
    return nullptr;
}

bool SymbolTable::remove(const std::string& name) {
    return symbols.erase(name) > 0;
}

void SymbolTable::print() const {
    std::cout << "Scope Level: " << scopeLevel << std::endl;
    for (const auto& [name, symbol] : symbols) {
        std::cout << "  " << name << " (type: ";
        switch (symbol->dataType) {
            case TokenType::INT_TYPE: std::cout << "int"; break;
            case TokenType::FLOAT_TYPE: std::cout << "float"; break;
            case TokenType::BOOL_TYPE: std::cout << "bool"; break;
            case TokenType::STRING_TYPE: std::cout << "string"; break;
            default: std::cout << "unknown";
        }
        std::cout << ", initialized: " << (symbol->isInitialized ? "yes" : "no");
        std::cout << ", constant: " << (symbol->isConstant ? "yes" : "no") << ")" << std::endl;
    }
    
    if (parent) {
        std::cout << "Parent scope:" << std::endl;
        parent->print();
    }
}