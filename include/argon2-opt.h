#ifndef ARGON2_OPT_H
#define ARGON2_OPT_H

#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Symbols visibility control */
#ifdef A2_VISCTL
#define ARGON2_PUBLIC __attribute__((visibility("default")))
#elif _MSC_VER
#define ARGON2_PUBLIC __declspec(dllexport)
#else
#define ARGON2_PUBLIC
#endif

/**
 * Selects the fastest available optimized implementation.
 * @param out The file for debug output (e. g. stderr; pass NULL for no
 * debug output)
 * @param prefix What to print before each line; NULL is equivalent to empty
 * string
 */
void argon2_select_impl(FILE *out, const char *prefix);

#if defined(__cplusplus)
}
#endif

#endif // ARGON2_OPT_H

