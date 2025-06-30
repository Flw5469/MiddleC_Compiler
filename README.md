# C-Like Language Compiler

A compiler that converts a Turing-complete C-like language into Intel x86 assembly code compatible with NASM on Windows 10.
I name this language "Middle C" because it is pretty mid (slang for mediocre) 

## Features

### Language Support
- **Variables**: Declaration and manipulation of variables with scope
- **Control Flow**: Nested branching (if) and while loops
- **Memory Management**: Dynamic memory allocation and deallocation
- **Arithmetic Operations**: Full arithmetic support with proper operator precedence
- **Turing Completeness**: Supports all fundamental programming constructs

### Compiler Architecture
- **Lexical Analysis**: Tokenizes source code into meaningful symbols
- **Syntax Analysis**: CYK (Cocke-Younger-Kasami) parsing algorithm for context-free grammar
- **Abstract Syntax Tree**: Constructs AST for intermediate representation
- **Code Generation**: Converts AST to Intel x86 assembly code
- **NASM Compatible**: Generated assembly works with NASM assembler on Windows 10

#### Variables
```c
var x=10;
x=x+5;
var y = 28;
y=y/x;
```

#### Control Flow
```c
if (condition) {
    // nested statements
    while (loop_condition) {
        // loop body
    }
}
```

#### Arithmetic with Precedence
```c
result = a + b * c - d / e;  // Proper operator precedence
```

#### Memory Allocation
```c
var peter ; peter = malloc 100 ; ( peter ) = 20 ; if ( ( * peter ) / 5 ) { function:show ; } ;
free ptr;
```


## Build Requirements

### Prerequisites
- **C++ Compiler**: GCC or MinGW with C++17 support
- **NASM**: Netwide Assembler for Windows
- **Windows 10**: Target platform
- **Make**: For build automation

### Building the Compiler
```bash
./build.bat
```

### Running the Compiler (input located at ./utilities)
```bash
./run.bat
```

### Complete Workflow

## Project Structure
C:.
│  build.bat
│  build_and_run.bat
│  makefile
│  readme.md
│  run.bat
│
├─build
│  │  main.exe
│  │
│  └─output
│          output.asm
│          output.exe
│          output.obj
│
├─dependencies
│  │  Goasm.zip
│  │  Golink.zip
│  │  Gorc.zip
│  │  nasm-2.16.03-installer-x64.exe
│  │
│  ├─Goasm
│  │      AdaptAsm21.zip
│  │      forhelp.txt
│  │      GoAsm.chm
│  │      GoAsm.exe
│  │
│  ├─Golink
│  │      GoLink.exe
│  │      GoLink.htm
│  │      GoLogo50.gif
│  │      mail6sblue.gif
│  │      up.gif
│  │
│  └─Gorc
│          GoLogo50.gif
│          GoRC.exe
│          GoRC.htm
│          mail6sblue.gif
│          up.gif
│
├─documentation
│      AST_example.png
│      generation_note.txt
│
├─other_scripts
│      index_matching.cpp
│      test.cpp
│      test.exe
│      test.s
│
├─src
│  │  assembly.cpp
│  │  cyk.cpp
│  │  helper_functions.cpp
│  │  main.cpp
│  │  parsing.cpp
│  │  rules.cpp
│  │  structures.cpp
│  │  testing.cpp
│  │  testing.exe
│  │  tree_functions.cpp
│  │
│  └─.vscode
│          c_cpp_properties.json
│          settings.json
│
├─unit_test
└─utilities
        asm_postfix.txt
        asm_prefix.txt
        input.txt


## Algorithm Details

### CYK Parser
- **Time Complexity**: O(n³|G|) where n is input length and |G| is grammar size
- **Space Complexity**: O(n²|G|) for the parsing table
- **Grammar Type**: Context-free grammars in Chomsky Normal Form
- **Parsing Strategy**: Bottom-up dynamic programming approach

### AST Construction
- **Tree Traversal**: Recursive descent through parse tree
- **Node Types**: Operators, operands, control structures, declarations
- **Semantic Analysis**: Scope resolution

### Code Generation
- **Traversal Method**: A Single Post-order traversal of AST
- **Register Allocation**: Simple register assignment strategy
- **Instruction Selection**: Direct mapping from AST nodes to assembly instructions

## Example

### Input (C-like code)
```c
var peter ; peter = malloc 100 ; ( peter ) = 20 ; if ( ( * peter ) / 5 ) { function:show ; } ;
```

### Output (Intel x86 Assembly)
```asm
assembly code:

; var peter
push 0
mov eax, ebp ; access the stack frame
mov eax, [eax-4]; load the variable into eax

; peter = malloc 100
mov eax, ebp ; access the stack frame
mov eax, [eax-4]; load the variable into eax
mov eax, 100
push eax
call malloc
mov ebx, eax
mov eax, ebp; access the stack frame
mov dword [eax-4], ebx; store ebx into memory

; ( peter ) = 20
mov eax, ebp ; access the stack frame
mov eax, [eax-4]; load the variable into eax
push eax
mov eax, 20
mov ebx, eax
pop eax
mov dword [eax], ebx

; if ( ( * peter ) / 5 ) { function:show ; }
mov eax, ebp ; access the stack frame
mov eax, [eax-4]; load the variable into eax
mov eax, [eax]      ; dereference the address
push eax
mov eax, 5
mov ebx, eax
pop eax
cdq
idiv ebx
cmp eax, 0             ; compare with 0
je if_end_1
push ebp          ; Save caller's base pointer
mov ebp, esp      ; Set up new base pointer
call show_function
mov esp, ebp      ; Restore stack pointer
pop ebp           ; Restore caller's base pointer
if_end_1:
```