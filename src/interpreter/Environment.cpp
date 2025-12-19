#include "Environment.h"
#include <iostream>
#include <stdexcept>

Environment::Environment(std::shared_ptr<Environment> parent) : parent(parent) {}

void Environment::define(const std::string& name, const Value& value) {
    values[name] = value;
}

void Environment::assign(const std::string& name, const Value& value) {
    if (values.find(name) != values.end()) {
        values[name] = value;
    } else if (parent) {
        parent->assign(name, value);
    } else {
        throw std::runtime_error("Undefined variable '" + name + "'");
    }
}

Value Environment::get(const std::string& name) {
    auto it = values.find(name);
    if (it != values.end()) {
        return it->second;
    }
    
    if (parent) {
        return parent->get(name);
    }
    
    throw std::runtime_error("Undefined variable '" + name + "'");
}

bool Environment::exists(const std::string& name) const {
    if (values.find(name) != values.end()) {
        return true;
    }
    
    if (parent) {
        return parent->exists(name);
    }
    
    return false;
}

bool Environment::isTruthy(const Value& value) {
    if (std::holds_alternative<nullptr_t>(value)) return false;
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value);
    if (std::holds_alternative<int>(value)) return std::get<int>(value) != 0;
    if (std::holds_alternative<float>(value)) return std::get<float>(value) != 0.0f;
    if (std::holds_alternative<std::string>(value)) return !std::get<std::string>(value).empty();
    return true;
}

bool Environment::isEqual(const Value& a, const Value& b) {
    if (std::holds_alternative<nullptr_t>(a) && std::holds_alternative<nullptr_t>(b)) return true;
    if (std::holds_alternative<nullptr_t>(a) || std::holds_alternative<nullptr_t>(b)) return false;
    
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) 
        return std::get<int>(a) == std::get<int>(b);
    if (std::holds_alternative<float>(a) && std::holds_alternative<float>(b)) 
        return std::get<float>(a) == std::get<float>(b);
    if (std::holds_alternative<bool>(a) && std::holds_alternative<bool>(b)) 
        return std::get<bool>(a) == std::get<bool>(b);
    if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b)) 
        return std::get<std::string>(a) == std::get<std::string>(b);
    
    // Cross-type comparisons
    if (std::holds_alternative<int>(a) && std::holds_alternative<float>(b)) 
        return static_cast<float>(std::get<int>(a)) == std::get<float>(b);
    if (std::holds_alternative<float>(a) && std::holds_alternative<int>(b)) 
        return std::get<float>(a) == static_cast<float>(std::get<int>(b));
    
    return false;
}

std::string Environment::valueToString(const Value& value) {
    if (std::holds_alternative<nullptr_t>(value)) return "null";
    if (std::holds_alternative<int>(value)) return std::to_string(std::get<int>(value));
    if (std::holds_alternative<float>(value)) return std::to_string(std::get<float>(value));
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value) ? "true" : "false";
    if (std::holds_alternative<std::string>(value)) return std::get<std::string>(value);
    return "unknown";
}

void Environment::print() const {
    std::cout << "Environment:" << std::endl;
    for (const auto& [name, value] : values) {
        std::cout << "  " << name << " = " << valueToString(value) << std::endl;
    }
    
    if (parent) {
        std::cout << "Parent environment:" << std::endl;
        parent->print();
    }
}