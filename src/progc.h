#ifndef PROGC_H
#define PROGC_H

/**
 * @defgroup ProgramCounterModule Program Counter Management
 * @brief Module for managing the program counter.
 *
 * This module provides functionality for managing a program counter used in
 * RISC-V compiler environment. It includes operations to get the current
 * program counter value, advance the counter by half words or full words, and
 * reset the counter to its initial state.
 *
 * @{
 */

/**
 * @brief Gets the current value of the program counter.
 *
 * This function returns the current value of the program counter, which can
 * be used to track the execution flow in a program.
 *
 * @return unsigned The current value of the program counter.
 */
extern unsigned get_pc(void);

/**
 * @brief Advances the program counter by one half word (2 bytes) for compressed instructions.
 *
 * This function increments the program counter by the size of a half word,
 * which is typically used to accommodate compressed instruction formats.
 *
 * @return unsigned int The new value of the program counter after advancement.
 */
extern unsigned int advance_half_word(void);

/**
 * @brief Advances the program counter by one full word (4 bytes).
 *
 * This function increments the program counter by the size of a full word.
 *
 * @return unsigned int The new value of the program counter after advancement.
 */
extern unsigned int advance_word(void);

/**
 * @brief Resets the program counter to zero.
 *
 * This function sets the program counter back to its initial state (zero),
 * effectively restarting the tracking of the program's execution flow.
 */
extern void reset_pc(void);

/** @} */ /* End of ProgramCounterModule */

#endif /* PROGC_H */

