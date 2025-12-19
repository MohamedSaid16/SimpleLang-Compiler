#include <iostream>
#include <vector>
#include <string>
#include "../include/lexer/Lexer.h"
#include "../include/lexer/Token.h"

void testLexer() {
    std::cout << "Running Lexer Tests...\n";
    std::cout << "=====================\n";
    
    // Test 1: Basic arithmetic
    {
        std::string source = "let x = 10 + 20;";
        Lexer lexer(source);
        
        std::vector<Token> tokens;
        Token token;
        do {
            token = lexer.nextToken();
            tokens.push_back(token);
            std::cout << token.toString() << std::endl;
        } while (token.type != TokenType::END_OF_FILE);
        
        std::cout << "Test 1: " << (tokens.size() == 9 ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 2: String literal
    {
        std::string source = "let name = \"John Doe\";";
        Lexer lexer(source);
        
        std::vector<Token> tokens;
        Token token;
        do {
            token = lexer.nextToken();
            tokens.push_back(token);
            std::cout << token.toString() << std::endl;
        } while (token.type != TokenType::END_OF_FILE);
        
        std::cout << "Test 2: " << (tokens.size() == 6 ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 3: Keywords and identifiers
    {
        std::string source = "if x > 10 then print(\"High\"); end";
        Lexer lexer(source);
        
        std::vector<Token> tokens;
        Token token;
        do {
            token = lexer.nextToken();
            tokens.push_back(token);
            std::cout << token.toString() << std::endl;
        } while (token.type != TokenType::END_OF_FILE);
        
        std::cout << "Test 3: " << (tokens.size() == 12 ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 4: Comments
    {
        std::string source = "# This is a comment\nlet x = 5 # Another comment";
        Lexer lexer(source);
        
        std::vector<Token> tokens;
        Token token;
        do {
            token = lexer.nextToken();
            tokens.push_back(token);
            std::cout << token.toString() << std::endl;
        } while (token.type != TokenType::END_OF_FILE);
        
        std::cout << "Test 4: " << (tokens.size() == 6 ? "PASSED" : "FAILED") << "\n\n";
    }
    
    // Test 5: Float numbers
    {
        std::string source = "let pi = 3.14159; let e = 2.71828;";
        Lexer lexer(source);
        
        std::vector<Token> tokens;
        Token token;
        do {
            token = lexer.nextToken();
            tokens.push_back(token);
            std::cout << token.toString() << std::endl;
        } while (token.type != TokenType::END_OF_FILE);
        
        std::cout << "Test 5: " << (tokens.size() == 10 ? "PASSED" : "FAILED") << "\n\n";
    }
    
    std::cout << "Lexer Tests Complete!\n";
}

int main() {
    testLexer();
    return 0;
}