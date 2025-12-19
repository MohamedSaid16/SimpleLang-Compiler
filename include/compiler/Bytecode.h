#ifndef BYTECODE_H
#define BYTECODE_H

#include <vector>
#include <cstdint>
#include <string>
#include "../lexer/Token.h"

enum class OpCode : uint8_t {
    // Constants
    LOAD_CONST, LOAD_NULL, LOAD_TRUE, LOAD_FALSE,
    
    // Variables
    LOAD_VAR, STORE_VAR, DECLARE_VAR,
    
    // Arithmetic
    ADD, SUB, MUL, DIV, MOD, NEG,
    
    // Comparison
    EQ, NEQ, LT, GT, LTE, GTE,
    
    // Logical
    AND, OR, NOT,
    
    // Control flow
    JUMP, JUMP_IF_FALSE, JUMP_IF_TRUE, CALL,
    RETURN, POP,
    
    // Built-in functions
    PRINT, INPUT,
    
    // Special
    HALT
};

struct Bytecode {
    OpCode opcode;
    std::vector<uint32_t> operands;
    
    Bytecode(OpCode opcode, std::vector<uint32_t> operands = {})
        : opcode(opcode), operands(operands) {}
};

class BytecodeWriter {
private:
    std::vector<uint8_t> code;
    std::vector<Value> constants;
    
public:
    void writeByte(uint8_t byte);
    void writeOpCode(OpCode opcode);
    void writeOperand(uint32_t operand);
    
    void addConstant(const Value& value);
    size_t addConstantGetIndex(const Value& value);
    
    const std::vector<uint8_t>& getCode() const { return code; }
    const std::vector<Value>& getConstants() const { return constants; }
    
    void disassemble() const;
    std::string opcodeToString(OpCode opcode) const;
};

#endif