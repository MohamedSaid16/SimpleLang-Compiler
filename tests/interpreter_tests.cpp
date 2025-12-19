#include <iostream>
#include <sstream>
#include <memory>
#include "../include/lexer/Lexer.h"
#include "../include/parser/Parser.h"
#include "../include/interpreter/Interpreter.h"
#include "../include/semantic/SemanticAnalyzer.h"

void captureOutput(std::function<void()> func, std::string& output) {
    // Save old cout buffer
    std::streambuf* oldCoutBuffer = std::cout.rdbuf();
    
    // Redirect cout to stringstream
    std::stringstream buffer;
    std::cout.rdbuf(buffer.rdbuf());
    
    // Run the function
    func();
    
    // Restore old cout buffer
    std::cout.rdbuf(oldCoutBuffer);
    
    // Get the output
    output = buffer.str();
}

void testInterpreter() {
    std::cout << "Running Interpreter Tests...\n";
    std::cout << "===========================\n";
    
    int passed = 0;
    int total = 0;
    
    // Test 1: Basic arithmetic
    {
        total++;
        std::string source = "let x = 10; let y = 20; let z = x + y; print(z);";
        
        Lexer lexer(source);
        Parser parser(lexer);
        auto program = parser.parse();
        
        if (!parser.hasErrors()) {
            SemanticAnalyzer analyzer;
            analyzer.analyze(program);
            
            if (!analyzer.hasErrors()) {
                std::string output;
                captureOutput([&]() {
                    Interpreter interpreter;
                    interpreter.interpret(program);
                }, output);
                
                if (output.find("30") != std::string::npos) {
                    std::cout << "Test 1: PASSED\n";
                    passed++;
                } else {
                    std::cout << "Test 1: FAILED - Output: " << output << "\n";
                }
            } else {
                std::cout << "Test 1: FAILED - Semantic errors\n";
            }
        } else {
            std::cout << "Test 1: FAILED - Parse errors\n";
        }
    }
    
    // Test 2: Conditionals
    {
        total++;
        std::string source = "let x = 15; if x > 10 then print(\"High\"); else print(\"Low\"); end";
        
        Lexer lexer(source);
        Parser parser(lexer);
        auto program = parser.parse();
        
        if (!parser.hasErrors()) {
            SemanticAnalyzer analyzer;
            analyzer.analyze(program);
            
            if (!analyzer.hasErrors()) {
                std::string output;
                captureOutput([&]() {
                    Interpreter interpreter;
                    interpreter.interpret(program);
                }, output);
                
                if (output.find("High") != std::string::npos) {
                    std::cout << "Test 2: PASSED\n";
                    passed++;
                } else {
                    std::cout << "Test 2: FAILED - Output: " << output << "\n";
                }
            } else {
                std::cout << "Test 2: FAILED - Semantic errors\n";
            }
        } else {
            std::cout << "Test 2: FAILED - Parse errors\n";
        }
    }
    
    // Test 3: While loop
    {
        total++;
        std::string source = "let i = 1; while i <= 3 do print(i); let i = i + 1; end";
        
        Lexer lexer(source);
        Parser parser(lexer);
        auto program = parser.parse();
        
        if (!parser.hasErrors()) {
            SemanticAnalyzer analyzer;
            analyzer.analyze(program);
            
            if (!analyzer.hasErrors()) {
                std::string output;
                captureOutput([&]() {
                    Interpreter interpreter;
                    interpreter.interpret(program);
                }, output);
                
                if (output.find("1") != std::string::npos && 
                    output.find("2") != std::string::npos && 
                    output.find("3") != std::string::npos) {
                    std::cout << "Test 3: PASSED\n";
                    passed++;
                } else {
                    std::cout << "Test 3: FAILED - Output: " << output << "\n";
                }
            } else {
                std::cout << "Test 3: FAILED - Semantic errors\n";
            }
        } else {
            std::cout << "Test 3: FAILED - Parse errors\n";
        }
    }
    
    // Test 4: String concatenation
    {
        total++;
        std::string source = "let a = \"Hello, \"; let b = \"World!\"; print(a + b);";
        
        Lexer lexer(source);
        Parser parser(lexer);
        auto program = parser.parse();
        
        if (!parser.hasErrors()) {
            SemanticAnalyzer analyzer;
            analyzer.analyze(program);
            
            if (!analyzer.hasErrors()) {
                std::string output;
                captureOutput([&]() {
                    Interpreter interpreter;
                    interpreter.interpret(program);
                }, output);
                
                if (output.find("Hello, World!") != std::string::npos) {
                    std::cout << "Test 4: PASSED\n";
                    passed++;
                } else {
                    std::cout << "Test 4: FAILED - Output: " << output << "\n";
                }
            } else {
                std::cout << "Test 4: FAILED - Semantic errors\n";
            }
        } else {
            std::cout << "Test 4: FAILED - Parse errors\n";
        }
    }
    
    // Test 5: Complex expression
    {
        total++;
        std::string source = "let x = 10; let y = 20; let z = (x + y) * 3 - 15 / 5; print(z);";
        
        Lexer lexer(source);
        Parser parser(lexer);
        auto program = parser.parse();
        
        if (!parser.hasErrors()) {
            SemanticAnalyzer analyzer;
            analyzer.analyze(program);
            
            if (!analyzer.hasErrors()) {
                std::string output;
                captureOutput([&]() {
                    Interpreter interpreter;
                    interpreter.interpret(program);
                }, output);
                
                if (output.find("87") != std::string::npos) {
                    std::cout << "Test 5: PASSED\n";
                    passed++;
                } else {
                    std::cout << "Test 5: FAILED - Output: " << output << "\n";
                }
            } else {
                std::cout << "Test 5: FAILED - Semantic errors\n";
            }
        } else {
            std::cout << "Test 5: FAILED - Parse errors\n";
        }
    }
    
    // Test 6: Boolean logic
    {
        total++;
        std::string source = "let a = true; let b = false; print(a && b); print(a || b); print(!a);";
        
        Lexer lexer(source);
        Parser parser(lexer);
        auto program = parser.parse();
        
        if (!parser.hasErrors()) {
            SemanticAnalyzer analyzer;
            analyzer.analyze(program);
            
            if (!analyzer.hasErrors()) {
                std::string output;
                captureOutput([&]() {
                    Interpreter interpreter;
                    interpreter.interpret(program);
                }, output);
                
                if (output.find("false") != std::string::npos && 
                    output.find("true") != std::string::npos) {
                    std::cout << "Test 6: PASSED\n";
                    passed++;
                } else {
                    std::cout << "Test 6: FAILED - Output: " << output << "\n";
                }
            } else {
                std::cout << "Test 6: FAILED - Semantic errors\n";
            }
        } else {
            std::cout << "Test 6: FAILED - Parse errors\n";
        }
    }
    
    // Test 7: Variable reassignment
    {
        total++;
        std::string source = "let x = 5; print(x); x = 10; print(x);";
        
        Lexer lexer(source);
        Parser parser(lexer);
        auto program = parser.parse();
        
        if (!parser.hasErrors()) {
            SemanticAnalyzer analyzer;
            analyzer.analyze(program);
            
            if (!analyzer.hasErrors()) {
                std::string output;
                captureOutput([&]() {
                    Interpreter interpreter;
                    interpreter.interpret(program);
                }, output);
                
                if (output.find("5") != std::string::npos && 
                    output.find("10") != std::string::npos) {
                    std::cout << "Test 7: PASSED\n";
                    passed++;
                } else {
                    std::cout << "Test 7: FAILED - Output: " << output << "\n";
                }
            } else {
                std::cout << "Test 7: FAILED - Semantic errors\n";
            }
        } else {
            std::cout << "Test 7: FAILED - Parse errors\n";
        }
    }
    
    std::cout << "\nInterpreter Tests Complete!\n";
    std::cout << "Passed: " << passed << "/" << total << " tests\n";
}

int main() {
    testInterpreter();
    return 0;
}