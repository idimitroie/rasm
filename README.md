
# RISC-V (RV32I) Assembler written in C with GNU Flex & Bison

A simple and fast assembler for risc-v architecture

## Goal of the project

To create a minimalistic and readable assembler for RISC-V.
Mainly for educational purposes.

## Building and running the code

To build the system run: `./run.sh`
To run it, run: `./build/rasm`

*NB: Flex and bison must be installed to build*

To build the documentation, run 

```
touch ./build && rm -r ./build && cmake -DCMAKE_BUILD_TYPE=Doc -B ./build && cmake --build ./build
firefox ./build/Doxyout/html/index.html
```

## Current state of the project

Right now the assembler only accepts from stdin, and spits
out the encoded instructions in a nice and readable format
for debugging.

Example (`./run.sh`):
```
# *** RISCV Assembler V 0.0.1
# *** RASM: pass_number = 1
# label_local "1"       pc = 0x00000004 line 3
# label_local "666"     pc = 0x00000004 line 4
# label_globl "loop"    pc = 0x0000000c line 7
# label_local "1"       pc = 0x0000001c line 12
# label_local "1"       pc = 0x00000020 line 13
# label_local "1"       pc = 0x00000024 line 14
# label_local "1"       pc = 0x00000028 line 15
# *** RASM: pass_number = 2
83 20 41 00 #    00000000       line 2
23 a0 21 00 #    00000004       line 5
# local identifier "1" at = 00000008, ofs = -4, absadr = 0x00000004     line 6
e3 9e 41 fe #    00000008       line 6
83 20 01 00 #    0000000c       line 8
93 00 31 00 #    00000010       line 9
# identifier "loop" at 0x00000014 offs -8 absaddr 0x0000000c    line 10
e3 8c 20 fe #    00000014       line 10
63 80 20 08 #    00000018       line 11
63 84 20 00 #    0000001c       line 12
# local identifier "1" at = 00000020, ofs = 0, absadr = 0x00000020      line 13
63 80 20 00 #    00000020       line 13
# local identifier "1" at = 00000024, ofs = 4, absadr = 0x00000028      line 14
63 82 20 00 #    00000024       line 14
# identifier "loop" at 0x00000028 offs -28 absaddr 0x0000000c   line 16
e3 82 20 fe #    00000028       line 16
# local identifier "666" at = 0000002c, ofs = -40, absadr = 0x00000004  line 18
e3 8c 62 fc #    0000002c       line 18
83 20 41 00 #    00000030       line 19
```

## TODO:

### RV31I Base Instructions

 - [x] Registers and register ABI
 - [x] ALU register-register instructions
 - [x] ALU Immidiate instructions
 - [x] Basic instruction implementation with decimal numbers
 - [x] Allow hex numbers
 - [x] Immidiate validation
 - [x] Conditional branch instructions
 - [x] Upper immediate instructions
 - [x] Unconditional jumps
 - [x] Indirect jump instruction
 - [x] Load instructions
 - [x] Store instructions
 - [ ] Memory ordering instructions

### Misc

 - [x] Lables
 - [ ] Symbol table
 - [ ] Psudo-instructions
 - [ ] Hi low macro

### Command line options

 - [ ] Interactive mode *(only mode, for the time beeing)*
 - [ ] Pretty print assembly table
 - [x] Verbosity
 - [x] Input file
 - [x] Output filename
 - [ ] Target

### Output formats
 - [x] DMF file  [SmithForth](https://dacvs.neocities.org/SF/)
 - [ ] Hex dump
 - [ ] ELF .o File


## Understanding the code base

The instructions are defined in the `scanner.l`, and passes down options for op-code
funct3, funct7, etc. And a token representing the instruction type (f.ex. `R_TYPE`).
This means that you only need to write one line to implement a new instruction.

Instruction types has to be defined in `parser.y`. A token and a grammar must be created for them.
In the `instr.c` there should be a generator function, like `instr_R_type`, used by the grammar,
There should also be a entry in the `enum instr_type` in `decode.h`, a pretty print function in
`decode.c`, like `print_instr_R_type`, and a case should be added in both the `print_instr` and
`get_instr_type` functions (also in `decode.c`).

The registers and register ABI is also defined in `scanner.l`.

Further types data types are also defined in `AST.h`.

With this knowladge the rest of the code base should be fairly easy to understand,
given knowledge about GNU Flex & Bison.
