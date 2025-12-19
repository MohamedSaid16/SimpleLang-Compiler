#ifndef STANDARDLIBRARY_H
#define STANDARDLIBRARY_H

#include <functional>
#include <vector>
#include <variant>
#include "../interpreter/Environment.h"

using NativeFunction = std::function<Value(const std::vector<Value>&)>;

struct StandardFunction {
    std::string name;
    std::vector<TokenType> parameterTypes;
    TokenType returnType;
    NativeFunction function;
};

class StandardLibrary {
private:
    static std::vector<StandardFunction> functions;
    
public:
    static void initialize();
    static void registerFunctions(std::shared_ptr<Environment> env);
    static bool isStandardFunction(const std::string& name);
    static StandardFunction getFunction(const std::string& name);
    
    // Individual standard functions
    static Value print(const std::vector<Value>& args);
    static Value input(const std::vector<Value>& args);
    static Value toString(const std::vector<Value>& args);
    static Value toInt(const std::vector<Value>& args);
    static Value toFloat(const std::vector<Value>& args);
    static Value length(const std::vector<Value>& args);
    static Value substring(const std::vector<Value>& args);
    static Value concat(const std::vector<Value>& args);
    
private:
    static void initializeFunctions();
};

#endif