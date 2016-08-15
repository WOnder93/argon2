#ifndef ARGON2_OPT_H
#define ARGON2_OPT_H

#include <stdio.h>

/**
 * Selects the fastest available optimized implementation.
 * @param out The file for debug output (e. g. stderr; pass NULL for no
 * debug output)
 */
void argon2_select_impl(FILE *out);

#endif // ARGON2_OPT_H

