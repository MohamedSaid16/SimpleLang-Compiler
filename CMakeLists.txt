cmake_minimum_required(VERSION 3.10)
project(SimpleLang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/lexer/Lexer.cpp
    src/lexer/Token.cpp
    src/parser/Parser.cpp
    src/parser/AST.cpp
    src/semantic/SymbolTable.cpp
    src/semantic/TypeChecker.cpp
    src/semantic/SemanticAnalyzer.cpp
    src/interpreter/Environment.cpp
    src/interpreter/Interpreter.cpp
    src/core/Error.cpp
    src/core/Utils.cpp
)

# Create executable
add_executable(simplelang ${SOURCES})

# Tests
enable_testing()
add_executable(run_tests
    tests/lexer_tests.cpp
    tests/parser_tests.cpp
    tests/interpreter_tests.cpp
    ${SOURCES}
)
add_test(NAME SimpleLangTests COMMAND run_tests)

# Installation
install(TARGETS simplelang DESTINATION bin)