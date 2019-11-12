/*! \file
 * This file contains the important global definitions for the CS24 Python
 * interpreter.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*! History file for Sub-Python */
#define SUBPYTHON_HISTORY ".subpython"

/*! Default initial size for realloc-growing arrays. */
#define INITIAL_SIZE 8

/* A handy macro to delineate an unreachable branch in switches. */
#define UNREACHABLE() \
    do { \
        fprintf(stderr, __FILE__ ":%d - unreachable!\n", __LINE__); \
        exit(-1); \
    } while (0)

extern bool interactive;

#endif /* CONFIG_H */
