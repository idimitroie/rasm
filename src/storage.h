#pragma once

#include "ast.h"

/**
 * @defgroup DMPFormatModule DMP Format Storage
 * @brief Module for storing compiled instructions and labels in DMP format.
 *
 * This module provides functionality to store compiled instructions and labels
 * in a human-readable format known as DMP (Disassembled Machine Presentation). 
 * The DMP format displays code and data as hexadecimal bytes, with annotations 
 * and comments following the '#' character. This format is designed for easy 
 * human review and can be processed further by command-line tools such as 
 * `cut` and `xxd` for conversion back to binary.
 *
 * Currently, only DMP format is supported.
 *
 * @{
 */

/**
 * @brief Adds an instruction to the storage.
 *
 * This function stores the given instruction (opcode) along with its current
 * program counter (curpc) and line number (curlineno) for future reference.
 *
 * @param opcode Pointer to the instruction structure to be stored.
 * @param curpc The current program counter value at which the instruction is located.
 * @param curlineno The line number in the source code where the instruction was found.
 */
extern void storage_add_instr(instr_t *opcode, unsigned curpc, unsigned curlineno);

/**
 * @brief Adds a global label to the storage.
 *
 * This function registers a global label with its associated current program counter
 * and line number for later reference in the DMP output.
 *
 * @param lbl Pointer to the string representing the global label.
 * @param curpc The current program counter value at which the label is located.
 * @param curlineno The line number in the source code where the label was defined.
 */
extern void storage_add_glob_label(char *lbl, unsigned curpc, unsigned curlineno);

/**
 * @brief Adds a local label to the storage.
 *
 * This function registers a local label (identified by its numerical value) 
 * with its current program counter and line number for later reference in the DMP output.
 *
 * @param lbl The numerical identifier of the local label.
 * @param curpc The current program counter value at which the label is located.
 * @param curlineno The line number in the source code where the label was defined.
 */
extern void storage_add_locl_label(unsigned lbl, unsigned curpc, unsigned curlineno);

/**
 * @brief Adds a global identifier to the storage.
 *
 * This function registers a global identifier (label) along with its current program
 * counter, offset, absolute address, and line number for later use in DMP output.
 *
 * @param lbl Pointer to the string representing the global identifier.
 * @param curpc The current program counter value at which the identifier is located.
 * @param ofs The offset associated with the identifier.
 * @param absadr The absolute address corresponding to the identifier.
 * @param curlineno The line number in the source code where the identifier was defined.
 */
extern void storage_add_glob_identifier(char *lbl, unsigned curpc, unsigned ofs, unsigned absadr, unsigned curlineno);

/**
 * @brief Adds a local identifier to the storage.
 *
 * This function registers a local identifier (label) with its numerical value,
 * along with the current program counter, offset, absolute address, and line 
 * number for later use in DMP output.
 *
 * @param lbl The numerical identifier of the local identifier.
 * @param curpc The current program counter value at which the identifier is located.
 * @param ofs The offset associated with the identifier.
 * @param absadr The absolute address corresponding to the identifier.
 * @param curlineno The line number in the source code where the identifier was defined.
 */
extern void storage_add_locl_identifier(int lbl, unsigned curpc, unsigned ofs, unsigned absadr, unsigned curlineno);

/** @} */ /* End of DMPFormatModule */

