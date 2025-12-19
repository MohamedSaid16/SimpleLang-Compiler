#include "Error.h"
#include <sstream>

std::string Error::toString() const {
    std::stringstream ss;
    ss << "[" << typeToString(type) << "] ";
    if (!stage.empty()) {
        ss << "(" << stage << ") ";
    }
    ss << message;
    if (line != -1) {
        ss << " at line " << line;
        if (column != -1) {
            ss << ":" << column;
        }
    }
    return ss.str();
}

std::string Error::typeToString(ErrorType type) {
    switch (type) {
        case ErrorType::LEXICAL: return "Lexical Error";
        case ErrorType::SYNTAX: return "Syntax Error";
        case ErrorType::SEMANTIC: return "Semantic Error";
        case ErrorType::RUNTIME: return "Runtime Error";
        case ErrorType::WARNING: return "Warning";
        default: return "Unknown Error";
    }
}

void ErrorReporter::report(const Error& error) {
    errors.push_back(error);
}

void ErrorReporter::report(ErrorType type, const std::string& message, int line, int column) {
    errors.push_back(Error(type, message, line, column));
}

void ErrorReporter::printAll() const {
    for (const auto& error : errors) {
        std::cout << error.toString() << std::endl;
    }
}