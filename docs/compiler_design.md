# Compiler Design

## Architecture
1. **Lexer**: Converts source code to tokens
2. **Parser**: Builds Abstract Syntax Tree (AST)
3. **Semantic Analyzer**: Type checking and validation
4. **Code Generator**: Produces bytecode
5. **Interpreter**: Executes bytecode

## Phases
### 1. Lexical Analysis
- Input: Source code
- Output: Token stream
- Responsibilities: Token recognition, error detection

### 2. Syntax Analysis
- Input: Token stream
- Output: Abstract Syntax Tree
- Responsibilities: Grammar validation, AST construction

### 3. Semantic Analysis
- Input: AST
- Output: Annotated AST
- Responsibilities: Type checking, scope resolution

### 4. Code Generation
- Input: Annotated AST
- Output: Bytecode
- Responsibilities: Instruction generation, optimization

### 5. Execution
- Input: Bytecode
- Output: Program results
- Responsibilities: Runtime execution, memory management