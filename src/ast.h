
/*
 * An Abstract Syntax Tree, or AST, is a tree representation of the source code of a computer program that conveys the structure of the source code. Each node in the tree represents a construct occurring in the source code.
 * 
 * During conversion to it's AST, only the structural and content-related details of the source code are preserved, and any additional details are discarded. Information that are preserved, and are vital to the ASTs purpose, are:
 * 
 * Variable types, and location of each variable declaration
 * Order and definition of executable statements
 * Left and right components of binary operations
 * Identifiers and their assigned values
 * What are ASTs used for?
 * ASTs are used to represent the structure of a program's source code for the compiler to use. An AST is usually the result of the syntax analysis phase of a compiler. It often serves as an intermediate representation of the program through several stages that the compiler requires, and has a strong impact on the final output of the compiler.
 */

#ifndef _AST_H
#define _AST_H

struct instr
{
    unsigned char *data;
};
typedef struct instr instr_t;

struct instr_codes
{
    int opcode, funct3, funct7;
};
typedef struct instr_codes instr_codes_t;

struct label_s
{
	char *name;
	unsigned address;
};
typedef struct label_s label_t;

#endif
