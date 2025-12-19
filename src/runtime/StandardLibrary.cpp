#include "StandardLibrary.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<StandardFunction> StandardLibrary::functions;

void StandardLibrary::initialize() {
    initializeFunctions();
}

void StandardLibrary::registerFunctions(std::shared_ptr<Environment> env) {
    initialize();
    
    for (const auto& func : functions) {
        // Create a native function wrapper
        Value nativeFunc = func.function;
        env->define(func.name, nativeFunc);
    }
}

bool StandardLibrary::isStandardFunction(const std::string& name) {
    initialize();
    
    for (const auto& func : functions) {
        if (func.name == name) {
            return true;
        }
    }
    return false;
}

StandardFunction StandardLibrary::getFunction(const std::string& name) {
    initialize();
    
    for (const auto& func : functions) {
        if (func.name == name) {
            return func;
        }
    }
    
    throw std::runtime_error("Standard function not found: " + name);
}

// Standard function implementations
Value StandardLibrary::print(const std::vector<Value>& args) {
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << Environment::valueToString(args[i]);
        if (i < args.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    return nullptr;
}

Value StandardLibrary::input(const std::vector<Value>& args) {
    if (!args.empty()) {
        print(args);
    }
    
    std::string line;
    std::getline(std::cin, line);
    return line;
}

Value StandardLibrary::toString(const std::vector<Value>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("toString() expects exactly 1 argument");
    }
    
    return Environment::valueToString(args[0]);
}

Value StandardLibrary::toInt(const std::vector<Value>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("toInt() expects exactly 1 argument");
    }
    
    const Value& arg = args[0];
    
    try {
        if (std::holds_alternative<int>(arg)) {
            return arg;
        }
        if (std::holds_alternative<float>(arg)) {
            return static_cast<int>(std::get<float>(arg));
        }
        if (std::holds_alternative<bool>(arg)) {
            return std::get<bool>(arg) ? 1 : 0;
        }
        if (std::holds_alternative<std::string>(arg)) {
            return std::stoi(std::get<std::string>(arg));
        }
    } catch (...) {
        throw std::runtime_error("Cannot convert to integer");
    }
    
    throw std::runtime_error("Unsupported type for conversion to integer");
}

Value StandardLibrary::toFloat(const std::vector<Value>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("toFloat() expects exactly 1 argument");
    }
    
    const Value& arg = args[0];
    
    try {
        if (std::holds_alternative<float>(arg)) {
            return arg;
        }
        if (std::holds_alternative<int>(arg)) {
            return static_cast<float>(std::get<int>(arg));
        }
        if (std::holds_alternative<bool>(arg)) {
            return std::get<bool>(arg) ? 1.0f : 0.0f;
        }
        if (std::holds_alternative<std::string>(arg)) {
            return std::stof(std::get<std::string>(arg));
        }
    } catch (...) {
        throw std::runtime_error("Cannot convert to float");
    }
    
    throw std::runtime_error("Unsupported type for conversion to float");
}

Value StandardLibrary::length(const std::vector<Value>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("length() expects exactly 1 argument");
    }
    
    const Value& arg = args[0];
    
    if (std::holds_alternative<std::string>(arg)) {
        return static_cast<int>(std::get<std::string>(arg).length());
    }
    
    throw std::runtime_error("length() expects a string argument");
}

Value StandardLibrary::substring(const std::vector<Value>& args) {
    if (args.size() != 3) {
        throw std::runtime_error("substring() expects exactly 3 arguments");
    }
    
    const Value& strArg = args[0];
    const Value& startArg = args[1];
    const Value& lengthArg = args[2];
    
    if (!std::holds_alternative<std::string>(strArg) ||
        !std::holds_alternative<int>(startArg) ||
        !std::holds_alternative<int>(lengthArg)) {
        throw std::runtime_error("substring() expects (string, int, int) arguments");
    }
    
    std::string str = std::get<std::string>(strArg);
    int start = std::get<int>(startArg);
    int len = std::get<int>(lengthArg);
    
    if (start < 0 || start >= str.length()) {
        throw std::runtime_error("Start index out of bounds");
    }
    
    if (len < 0 || start + len > str.length()) {
        len = str.length() - start;
    }
    
    return str.substr(start, len);
}

Value StandardLibrary::concat(const std::vector<Value>& args) {
    if (args.empty()) {
        return "";
    }
    
    std::string result;
    for (const auto& arg : args) {
        result += Environment::valueToString(arg);
    }
    
    return result;
}

void StandardLibrary::initializeFunctions() {
    if (!functions.empty()) {
        return; // Already initialized
    }
    
    functions = {
        {"print", {}, TokenType::VOID_TYPE, print},
        {"input", {}, TokenType::STRING_TYPE, input},
        {"toString", {TokenType::INT_TYPE}, TokenType::STRING_TYPE, toString},
        {"toInt", {TokenType::STRING_TYPE}, TokenType::INT_TYPE, toInt},
        {"toFloat", {TokenType::STRING_TYPE}, TokenType::FLOAT_TYPE, toFloat},
        {"length", {TokenType::STRING_TYPE}, TokenType::INT_TYPE, length},
        {"substring", {TokenType::STRING_TYPE, TokenType::INT_TYPE, TokenType::INT_TYPE}, 
         TokenType::STRING_TYPE, substring},
        {"concat", {TokenType::STRING_TYPE, TokenType::STRING_TYPE}, 
         TokenType::STRING_TYPE, concat}
    };
}