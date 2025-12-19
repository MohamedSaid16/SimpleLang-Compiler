#include <iostream>
#include <fstream>
#include <string>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "semantic/SemanticAnalyzer.h"
#include "interpreter/Interpreter.h"
#include "core/Utils.h"
#include "core/Error.h"

void run(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer);
    
    auto program = parser.parse();
    
    if (parser.hasErrors()) {
        std::cout << "Parser errors:" << std::endl;
        Utils::printErrors(parser.getErrors());
        return;
    }
    
    SemanticAnalyzer analyzer;
    analyzer.analyze(program);
    
    if (analyzer.hasErrors()) {
        std::cout << "Semantic errors:" << std::endl;
        Utils::printErrors(analyzer.getErrors());
        return;
    }
    
    Interpreter interpreter;
    interpreter.interpret(program);
    
    if (interpreter.hasErrors()) {
        std::cout << "Runtime errors:" << std::endl;
        Utils::printErrors(interpreter.getErrors());
    }
}

void runFile(const std::string& filename) {
    try {
        std::string source = Utils::readFile(filename);
        run(source);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void runPrompt() {
    std::string line;
    std::cout << "SimpleLang REPL (type 'exit' to quit)" << std::endl;
    
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        
        if (line == "exit" || line == "quit") {
            break;
        }
        
        run(line);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: simplelang [script]" << std::endl;
        return 1;
    }
    
    if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }
    
    return 0;
}