#include <iostream>
#include <memory>
#include "../include/lexer/Lexer.h"
#include "../include/parser/Parser.h"
#include "../include/parser/AST.h"

void testParser() {
    std::cout << "Running Parser Tests...\n";
    std::cout << "======================\n";
    
    // Test 1: Variable declaration
    {
        std::string source = "let x = 10;";
        Lexer lexer(source);
        Parser parser(lexer);
        
        auto program = parser.parse();
        
        if (parser.hasErrors()) {
            std::cout << "Test 1 Errors:\n";
            for (const auto& error : parser.getErrors()) {
                std::cout << error.toString() << std::endl;
            }
        }
        
        std::cout << "Test 1: " << (!parser.hasErrors() && program ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 2: Arithmetic expression
    {
        std::string source = "let result = (10 + 20) * 3 / 5;";
        Lexer lexer(source);
        Parser parser(lexer);
        
        auto program = parser.parse();
        
        if (parser.hasErrors()) {
            std::cout << "Test 2 Errors:\n";
            for (const auto& error : parser.getErrors()) {
                std::cout << error.toString() << std::endl;
            }
        }
        
        std::cout << "Test 2: " << (!parser.hasErrors() && program ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 3: If statement
    {
        std::string source = "if x > 10 then print(\"High\"); else print(\"Low\"); end";
        Lexer lexer(source);
        Parser parser(lexer);
        
        auto program = parser.parse();
        
        if (parser.hasErrors()) {
            std::cout << "Test 3 Errors:\n";
            for (const auto& error : parser.getErrors()) {
                std::cout << error.toString() << std::endl;
            }
        }
        
        std::cout << "Test 3: " << (!parser.hasErrors() && program ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 4: While loop
    {
        std::string source = "while i < 10 do print(i); let i = i + 1; end";
        Lexer lexer(source);
        Parser parser(lexer);
        
        auto program = parser.parse();
        
        if (parser.hasErrors()) {
            std::cout << "Test 4 Errors:\n";
            for (const auto& error : parser.getErrors()) {
                std::cout << error.toString() << std::endl;
            }
        }
        
        std::cout << "Test 4: " << (!parser.hasErrors() && program ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 5: Function declaration
    {
        std::string source = "function add(a: int, b: int): int { return a + b; }";
        Lexer lexer(source);
        Parser parser(lexer);
        
        auto program = parser.parse();
        
        if (parser.hasErrors()) {
            std::cout << "Test 5 Errors:\n";
            for (const auto& error : parser.getErrors()) {
                std::cout << error.toString() << std::endl;
            }
        }
        
        std::cout << "Test 5: " << (!parser.hasErrors() && program ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 6: Print statement with multiple arguments
    {
        std::string source = "print(\"Value:\", x, \"Result:\", x * 2);";
        Lexer lexer(source);
        Parser parser(lexer);
        
        auto program = parser.parse();
        
        if (parser.hasErrors()) {
            std::cout << "Test 6 Errors:\n";
            for (const auto& error : parser.getErrors()) {
                std::cout << error.toString() << std::endl;
            }
        }
        
        std::cout << "Test 6: " << (!parser.hasErrors() && program ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 7: Complex expression
    {
        std::string source = "let x = 10 + 20 * 3 - 15 / 5 % 2;";
        Lexer lexer(source);
        Parser parser(lexer);
        
        auto program = parser.parse();
        
        if (parser.hasErrors()) {
            std::cout << "Test 7 Errors:\n";
            for (const auto& error : parser.getErrors()) {
                std::cout << error.toString() << std::endl;
            }
        }
        
        std::cout << "Test 7: " << (!parser.hasErrors() && program ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 8: Assignment
    {
        std::string source = "let x = 10; x = x + 5;";
        Lexer lexer(source);
        Parser parser(lexer);
        
        auto program = parser.parse();
        
        if (parser.hasErrors()) {
            std::cout << "Test 8 Errors:\n";
            for (const auto& error : parser.getErrors()) {
                std::cout << error.toString() << std::endl;
            }
        }
        
        std::cout << "Test 8: " << (!parser.hasErrors() && program ? "PASSED" : "FAILED") << "\n\n";
    }
    
    std::cout << "Parser Tests Complete!\n";
}

int main() {
    testParser();
    return 0;
}