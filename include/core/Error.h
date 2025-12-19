#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <vector>
#include "../lexer/Token.h"

enum class ErrorType {
    LEXICAL,
    SYNTAX,
    SEMANTIC,
    RUNTIME,
    WARNING
};

struct Error {
    ErrorType type;
    std::string message;
    int line;
    int column;
    std::string stage;
    
    Error(ErrorType type, const std::string& message, int line = -1, int column = -1, 
          const std::string& stage = "")
        : type(type), message(message), line(line), column(column), stage(stage) {}
    
    std::string toString() const;
    static std::string typeToString(ErrorType type);
};

class ErrorReporter {
private:
    std::vector<Error> errors;
    
public:
    void report(const Error& error);
    void report(ErrorType type, const std::string& message, int line = -1, int column = -1);
    bool hasErrors() const { return !errors.empty(); }
    const std::vector<Error>& getErrors() const { return errors; }
    void clear() { errors.clear(); }
    void printAll() const;
};

#endif