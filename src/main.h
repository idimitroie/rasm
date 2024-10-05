#ifndef _MAIN_H
#define _MAIN_H

#include "ast.h"

/**
 * @defgroup MainModule Main Application Module
 * @brief Main module for the RISC-V assembler application.
 *
 * This module serves as the main entry point for the RISC-V assembler application.
 * It includes global variables and functions necessary for the assembler's 
 * operation, including verbosity control and error handling.
 *
 * @{
 */

/**
 * @brief Global variable for debugging verbosity level.
 *
 * This variable controls the verbosity of debug output throughout the application.
 * Increasing the value of this variable enables more detailed logging of 
 * operations and processes within the assembler.
 */
extern int debug;

/**
 * @brief Error reporting function.
 *
 * This function is called to report errors during the parsing or 
 * compilation processes. It typically outputs an error message to
 * the standard error stream, providing information about the issue.
 */
extern void yyerror();

/**
 * @brief Deprecated function to add an instruction.
 *
 * This function was previously used to print out the compiled opcode 
 * of the instruction. It is deprecated and should be replaced with 
 * more appropriate logging mechanisms in the future.
 *
 * @param instr Pointer to the instruction structure to be added.
 */
extern void add_instr(instr_t *instr);

/** @brief Custom printf function
 *
 * This function takes a format string and a variable number of arguments.
 * It checks if output_stream is NULL, and if so, defaults to stdout.
 * It uses va_list to handle the variable arguments and vfprintf() to write the
 * formatted output to the specified FILE*.
 */
extern void dmp_print(const char *format, ...);

/** @} */ /* End of MainModule */

#endif /* _MAIN_H */

