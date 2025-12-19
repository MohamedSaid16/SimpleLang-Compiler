#include "Bytecode.h"
#include <iostream>
#include <iomanip>

void BytecodeWriter::writeByte(uint8_t byte) {
    code.push_back(byte);
}

void BytecodeWriter::writeOpCode(OpCode opcode) {
    writeByte(static_cast<uint8_t>(opcode));
}

void BytecodeWriter::writeOperand(uint32_t operand) {
    // Write operand as 4 bytes (big-endian)
    writeByte((operand >> 24) & 0xFF);
    writeByte((operand >> 16) & 0xFF);
    writeByte((operand >> 8) & 0xFF);
    writeByte(operand & 0xFF);
}

void BytecodeWriter::addConstant(const Value& value) {
    constants.push_back(value);
}

size_t BytecodeWriter::addConstantGetIndex(const Value& value) {
    // Check if constant already exists
    for (size_t i = 0; i < constants.size(); i++) {
        if (constants[i] == value) {
            return i;
        }
    }
    
    // Add new constant
    constants.push_back(value);
    return constants.size() - 1;
}

void BytecodeWriter::disassemble() const {
    std::cout << "Bytecode (" << code.size() << " bytes):\n";
    std::cout << "========================\n";
    
    size_t offset = 0;
    while (offset < code.size()) {
        std::cout << std::setw(4) << std::setfill('0') << offset << "  ";
        
        OpCode opcode = static_cast<OpCode>(code[offset]);
        std::cout << opcodeToString(opcode);
        offset++;
        
        // Handle operands based on opcode
        switch (opcode) {
            case OpCode::LOAD_CONST:
            case OpCode::LOAD_VAR:
            case OpCode::STORE_VAR:
            case OpCode::DECLARE_VAR:
            case OpCode::JUMP:
            case OpCode::JUMP_IF_FALSE:
            case OpCode::JUMP_IF_TRUE:
            case OpCode::CALL:
                if (offset + 4 <= code.size()) {
                    uint32_t operand = (code[offset] << 24) | 
                                      (code[offset + 1] << 16) | 
                                      (code[offset + 2] << 8) | 
                                      code[offset + 3];
                    std::cout << " " << operand;
                    offset += 4;
                }
                break;
                
            default:
                // No operands
                break;
        }
        
        std::cout << "\n";
    }
    
    // Print constants pool
    if (!constants.empty()) {
        std::cout << "\nConstants pool (" << constants.size() << " constants):\n";
        std::cout << "========================\n";
        for (size_t i = 0; i < constants.size(); i++) {
            std::cout << "  [" << i << "] = ";
            // Would need to implement value printing
            std::cout << "constant\n";
        }
    }
}

std::string BytecodeWriter::opcodeToString(OpCode opcode) const {
    switch (opcode) {
        case OpCode::LOAD_CONST: return "LOAD_CONST";
        case OpCode::LOAD_NULL: return "LOAD_NULL";
        case OpCode::LOAD_TRUE: return "LOAD_TRUE";
        case OpCode::LOAD_FALSE: return "LOAD_FALSE";
        case OpCode::LOAD_VAR: return "LOAD_VAR";
        case OpCode::STORE_VAR: return "STORE_VAR";
        case OpCode::DECLARE_VAR: return "DECLARE_VAR";
        case OpCode::ADD: return "ADD";
        case OpCode::SUB: return "SUB";
        case OpCode::MUL: return "MUL";
        case OpCode::DIV: return "DIV";
        case OpCode::MOD: return "MOD";
        case OpCode::NEG: return "NEG";
        case OpCode::EQ: return "EQ";
        case OpCode::NEQ: return "NEQ";
        case OpCode::LT: return "LT";
        case OpCode::GT: return "GT";
        case OpCode::LTE: return "LTE";
        case OpCode::GTE: return "GTE";
        case OpCode::AND: return "AND";
        case OpCode::OR: return "OR";
        case OpCode::NOT: return "NOT";
        case OpCode::JUMP: return "JUMP";
        case OpCode::JUMP_IF_FALSE: return "JUMP_IF_FALSE";
        case OpCode::JUMP_IF_TRUE: return "JUMP_IF_TRUE";
        case OpCode::CALL: return "CALL";
        case OpCode::RETURN: return "RETURN";
        case OpCode::POP: return "POP";
        case OpCode::PRINT: return "PRINT";
        case OpCode::INPUT: return "INPUT";
        case OpCode::HALT: return "HALT";
        default: return "UNKNOWN";
    }
}