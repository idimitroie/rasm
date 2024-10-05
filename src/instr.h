
#ifndef _INSTR_H
#define _INSTR_H

#include <stdlib.h>
#include <stdio.h>


#include "ast.h"
#include "parser.tab.h"
#include "main.h"

/**
 * @defgroup InstructionEncodingModule Instruction Encoding
 * @brief Module for encoding RISC-V instructions.
 *
 * This module provides functionality to create and encode RISC-V instructions
 * in various formats, including R-type, I-type, B-type, U-type, J-type, and
 * S-type instructions. Each function constructs an instruction based on its
 * specific format, populating the instruction structure with the relevant
 * opcode and operand information. This is essential for generating machine
 * code from assembly language inputs.
 *
 * @{
 */

/**
 * @brief Creates an R-type instruction.
 *
 * This function constructs an R-type instruction with the specified opcode,
 * destination register, and source registers.
 *
 * @param instr_codes The opcode of the instruction.
 * @param rd The destination register.
 * @param rs1 The first source register.
 * @param rs2 The second source register.
 * @return instr_t* A pointer to the created R-type instruction.
 */
extern instr_t *instr_R_type(instr_codes_t instr_codes, int rd, int rs1, int rs2);

/**
 * @brief Creates an I-type instruction.
 *
 * This function constructs an I-type instruction with the specified opcode,
 * destination register, source register, and immediate value.
 *
 * @param instr_codes The opcode of the instruction.
 * @param rd The destination register.
 * @param rs1 The source register.
 * @param immi The immediate value to be used in the instruction.
 * @return instr_t* A pointer to the created I-type instruction.
 */
extern instr_t *instr_I_type(instr_codes_t instr_codes, int rd, int rs1, int immi);

/**
 * @brief Creates an I_SHAMT-type instruction.
 *
 * This function constructs an I-type instruction that includes a shift amount
 * with the specified opcode, destination register, source register, and
 * immediate value.
 *
 * @param instr_codes The opcode of the instruction.
 * @param rd The destination register.
 * @param rs1 The source register.
 * @param immi The shift amount to be used in the instruction.
 * @return instr_t* A pointer to the created I_SHAMT-type instruction.
 */
extern instr_t *instr_I_SHAMT_type(instr_codes_t instr_codes, int rd, int rs1, int immi);

/**
 * @brief Creates a B-type instruction.
 *
 * This function constructs a B-type instruction with the specified opcode,
 * source registers, and immediate value for branching.
 *
 * @param instr_codes The opcode of the instruction.
 * @param rs1 The first source register.
 * @param rs2 The second source register.
 * @param immi The immediate value to determine the branch target.
 * @return instr_t* A pointer to the created B-type instruction.
 */
extern instr_t *instr_B_type(instr_codes_t instr_codes, int rs1, int rs2, int immi);

/**
 * @brief Creates a U-type instruction.
 *
 * This function constructs a U-type instruction with the specified opcode,
 * destination register, and immediate value.
 *
 * @param instr_codes The opcode of the instruction.
 * @param rd The destination register.
 * @param immi The immediate value to be used in the instruction.
 * @return instr_t* A pointer to the created U-type instruction.
 */
extern instr_t *instr_U_type(instr_codes_t instr_codes, int rd, int immi);

/**
 * @brief Creates a J-type instruction.
 *
 * This function constructs a J-type instruction with the specified opcode,
 * destination register, and immediate value for jumps.
 *
 * @param instr_codes The opcode of the instruction.
 * @param rd The destination register.
 * @param immi The jump target immediate value.
 * @return instr_t* A pointer to the created J-type instruction.
 */
extern instr_t *instr_J_type(instr_codes_t instr_codes, int rd, int immi);

/**
 * @brief Creates an S-type instruction.
 *
 * This function constructs an S-type instruction with the specified opcode,
 * source register, immediate value, and the second source register.
 *
 * @param instr_codes The opcode of the instruction.
 * @param rs1 The source register.
 * @param immi The immediate value for memory offset.
 * @param rs2 The second source register for the store operation.
 * @return instr_t* A pointer to the created S-type instruction.
 */
extern instr_t *instr_S_type(instr_codes_t instr_codes, int rs1, int immi, int rs2);

/** @} */ // End of InstructionEncodingModule

#endif /* _INSTR_H */

