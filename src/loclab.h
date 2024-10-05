#include <stdbool.h>

/**
 * @defgroup LocalLabelModule Local Label Management
 * @brief Module for managing local labels in assembly-like branching.
 *
 * This module provides functionality to handle local labels, which are typically
 * used in assembly code for branching operations. Local labels are identified by
 * numbers (e.g., `1:` or `2:`) and can be referenced either forward (`name f`) or
 * backward (`name b`) relative to the current program counter (PC). These labels
 * facilitate compact and reusable branching without the need for globally unique
 * names.
 *
 * The module provides APIs to initialize, add, query, and clean up local labels.
 * Initialization ensures that resources are set up, and a cleanup routine is
 * registered using `atexit()` for proper resource deallocation upon program exit.
 * 
 * @{
 */

/**
 * @brief Initializes the local label management system.
 *
 * This function sets up the system for managing local labels, typically used
 * in scenarios like branch labels (e.g., `1:` or `2:`). It also registers
 * the `loclab_deinit()` function to be automatically called upon program exit
 * using `atexit()`. This ensures that the system is properly cleaned up when
 * the program terminates.
 */
extern void loclab_init(void);

/**
 * @brief Deinitializes the local label management system.
 *
 * Cleans up resources allocated for managing local labels. This function is
 * typically called automatically at program termination due to its registration
 * with `atexit()` in `loclab_init()`.
 */
extern void loclab_deinit(void);

/**
 * @brief Retrieves the address associated with a local label.
 *
 * This function searches for a local label by its name and program counter (PC)
 * and, if found, retrieves the associated address and stores it in the variable
 * pointed to by `addr`.
 *
 * Local labels use numbers (like `1:`, `2:`) and can be forward (`name f`) or
 * backward (`name b`) references, relative to the current program counter (PC).
 *
 * @param addr Pointer to an unsigned int where the address will be stored.
 * @param name The numerical name of the local label (e.g., `1` or `2`).
 * @param pc The current program counter, used to resolve forward/backward references.
 * @return _Bool Returns `true` if the label was found and the address retrieved,
 *              otherwise returns `false`.
 */
extern _Bool loclab_getpc(unsigned *addr, int name, unsigned pc);

/**
 * @brief Adds a local label and its associated address to the system.
 *
 * This function registers a local label (identified by a numerical `name`)
 * and associates it with the provided address. Local labels are typically
 * used for branching operations and are referenced relative to the current
 * program counter (PC).
 *
 * @param name The numerical name of the local label (e.g., `1` or `2`).
 * @param adr The address to associate with the local label.
 */
extern void loclab_add(unsigned int name, unsigned int adr);

/** @} */ /* End of LocalLabelModule */

