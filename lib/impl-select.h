#ifndef ARGON2_IMPL_SELECT_H
#define ARGON2_IMPL_SELECT_H

#include "core.h"

typedef struct Argon2_impl {
    const char *name;
    int (*check)(void);
    void (*fill_segment)(const argon2_instance_t *instance,
                         argon2_position_t position);
} argon2_impl;

typedef struct Argon2_impl_list {
    const argon2_impl *entries;
    size_t count;
} argon2_impl_list;

void argon2_get_impl_list(argon2_impl_list *list);
void fill_segment_default(const argon2_instance_t *instance,
                          argon2_position_t position);

/* TODO: pass Blake2 state as a ptr to a struct */
typedef struct Blake2_impl {
    const char *name;
    int (*check)(void);
    void (*blake2_update_block)(uint64_t *state, const void *block,
                                const uint64_t *t, uint64_t f0);
} blake2_impl;

typedef struct Blake2_impl_list {
    const blake2_impl *entries;
    size_t count;
} blake2_impl_list;

void blake2_get_impl_list(blake2_impl_list *list);
void blake2_update_block_default(uint64_t *state, const void *block,
                                 const uint64_t *t, uint64_t f0);

#endif // ARGON2_IMPL_SELECT_H

