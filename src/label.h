#pragma once
#include <stdbool.h>
/**
 * @defgroup GlobalLabelModule Global Label Management
 * @brief Module for managing global labels in the program.
 *
 * This module provides functionality to handle global labels, which are
 * typically used for marking addresses or sections of code with unique
 * identifiers. Unlike local labels, global labels are not limited to
 * specific code blocks and can be referenced across different parts of
 * the program.
 *
 * The module allows you to initialize the global label database, add new
 * labels associated with specific addresses, check if a label exists,
 * and retrieve the address associated with a given label. 
 * 
 * Initialization (`label_init()`) ensures that the database is ready for use
 * and registers the cleanup function (`label_deinit()`) using `atexit()` to
 * automatically release resources upon program termination.
 *
 * @{
 */

/**
 * @brief Initializes the label database.
 *
 * This function sets up the label management database and registers the
 * `label_deinit()` function to be called automatically upon program exit
 * using `atexit()`. This ensures that the label database is properly
 * cleaned up when the program terminates.
 *
 * @return int Returns 0 on success, or a negative error code on failure.
 */
extern int label_init(void);

/**
 * @brief Deinitializes the label database.
 *
 * This function cleans up resources allocated by the label database. It is
 * typically called automatically upon program exit if the `label_init()`
 * function was called earlier, thanks to its registration with `atexit()`.
 *
 * @return int Returns 0 on success, or a negative error code on failure.
 */
extern int label_deinit(void);

/**
 * @brief Adds a label associated with a specific address.
 *
 * This function associates a given label (represented by a string) with
 * a specified address. The label can later be used to retrieve the address
 * or perform other operations on it.
 *
 * @param lbl The label string to be added.
 * @param addr The address to be associated with the label.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
extern int label_add(const char *lbl, unsigned addr);

/**
 * @brief Retrieves the address associated with a label.
 *
 * This function fetches the address associated with a given label and
 * stores it in the variable pointed to by `addr`. It returns `true` if
 * the label exists and the address was successfully retrieved, otherwise
 * returns `false`.
 *
 * @param addr Pointer to an unsigned int where the address will be stored.
 * @param lbl The label string for which to retrieve the associated address.
 * @return _Bool Returns `true` if the label exists and the address was retrieved,
 *              otherwise returns `false`.
 */
extern _Bool label_getpc(unsigned int *addr, const char *lbl);

/**
 * @brief Checks if a label exists in the database.
 *
 * This function checks if a label with the specified name exists within
 * the label database.
 *
 * @param lbl The label string to check for existence.
 * @return int Returns 1 if the label exists, 0 if it does not, or a negative
 *             error code on failure.
 */
extern int label_exist(const char *);

/** @} */ /* End of GlobalLabelModule */

#ifdef UNITTEST
struct labelList_s ;
struct label_s;
struct unittest_s
{
    void (*freeLabelList)(struct labelList_s *list);
    struct label_s *(*findLabel)(struct labelList_s *list, const char *name);
    void (*addLabel)(struct labelList_s *list, const char *name, int address);
    void (*resizeLabelList)(struct labelList_s *list);
    struct labelList_s *(*createLabelList)();

    struct labelList_s **root_list;
};
extern struct unittest_s unittest;

#ifdef __GNUC__
#  pragma GCC warning "Compiled for unit testing. This is a non-critical GCC-specific warning"
#endif

#endif

