#ifndef ARGON2_BLAKE2_OPT_H
#define ARGON2_BLAKE2_OPT_H

#include "impl-select.h"

blake2_impl selected_blake2_impl;

static inline void blake2b_compress(uint64_t *state, const void *block,
                                    const uint64_t *t, uint64_t f)
{
    selected_blake2_impl.blake2_update_block(state, block, t, f);
}

#endif // ARGON2_BLAKE2_OPT_H
