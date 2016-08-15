#include "impl-select.h"

#include "blake2/blake2-impl.h"
#include "blake2/blake2-common.h"

static const unsigned int blake2b_sigma[12][16] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
    {11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4},
    {7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
    {9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13},
    {2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9},
    {12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11},
    {13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10},
    {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5},
    {10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
};

#define G(m, r, i, a, b, c, d) \
    do { \
        a = a + b + m[blake2b_sigma[r][2 * i + 0]]; \
        d = rotr64(d ^ a, 32); \
        c = c + d; \
        b = rotr64(b ^ c, 24); \
        a = a + b + m[blake2b_sigma[r][2 * i + 1]]; \
        d = rotr64(d ^ a, 16); \
        c = c + d; \
        b = rotr64(b ^ c, 63); \
    } while ((void)0, 0)

#define ROUND(m, v, r) \
    do { \
        G(m, r, 0, v[0], v[4], v[ 8], v[12]); \
        G(m, r, 1, v[1], v[5], v[ 9], v[13]); \
        G(m, r, 2, v[2], v[6], v[10], v[14]); \
        G(m, r, 3, v[3], v[7], v[11], v[15]); \
        G(m, r, 4, v[0], v[5], v[10], v[15]); \
        G(m, r, 5, v[1], v[6], v[11], v[12]); \
        G(m, r, 6, v[2], v[7], v[ 8], v[13]); \
        G(m, r, 7, v[3], v[4], v[ 9], v[14]); \
    } while ((void)0, 0)

void blake2_update_block_default(uint64_t *state, const void *block,
                                 const uint64_t *t, uint64_t f0)
{
    uint64_t m[16];
    uint64_t v[16];

    m[ 0] = load64((const uint64_t *)block +  0);
    m[ 1] = load64((const uint64_t *)block +  1);
    m[ 2] = load64((const uint64_t *)block +  2);
    m[ 3] = load64((const uint64_t *)block +  3);
    m[ 4] = load64((const uint64_t *)block +  4);
    m[ 5] = load64((const uint64_t *)block +  5);
    m[ 6] = load64((const uint64_t *)block +  6);
    m[ 7] = load64((const uint64_t *)block +  7);
    m[ 8] = load64((const uint64_t *)block +  8);
    m[ 9] = load64((const uint64_t *)block +  9);
    m[10] = load64((const uint64_t *)block + 10);
    m[11] = load64((const uint64_t *)block + 11);
    m[12] = load64((const uint64_t *)block + 12);
    m[13] = load64((const uint64_t *)block + 13);
    m[14] = load64((const uint64_t *)block + 14);
    m[15] = load64((const uint64_t *)block + 15);

    v[ 0] = state[0];
    v[ 1] = state[1];
    v[ 2] = state[2];
    v[ 3] = state[3];
    v[ 4] = state[4];
    v[ 5] = state[5];
    v[ 6] = state[6];
    v[ 7] = state[7];
    v[ 8] = blake2b_IV[0];
    v[ 9] = blake2b_IV[1];
    v[10] = blake2b_IV[2];
    v[11] = blake2b_IV[3];
    v[12] = blake2b_IV[4] ^ t[0];
    v[13] = blake2b_IV[5] ^ t[1];
    v[14] = blake2b_IV[6] ^ f0;
    v[15] = blake2b_IV[7];

    ROUND(m, v, 0);
    ROUND(m, v, 1);
    ROUND(m, v, 2);
    ROUND(m, v, 3);
    ROUND(m, v, 4);
    ROUND(m, v, 5);
    ROUND(m, v, 6);
    ROUND(m, v, 7);
    ROUND(m, v, 8);
    ROUND(m, v, 9);
    ROUND(m, v, 10);
    ROUND(m, v, 11);

    state[0] ^= v[0] ^ v[ 8];
    state[1] ^= v[1] ^ v[ 9];
    state[2] ^= v[2] ^ v[10];
    state[3] ^= v[3] ^ v[11];
    state[4] ^= v[4] ^ v[12];
    state[5] ^= v[5] ^ v[13];
    state[6] ^= v[6] ^ v[14];
    state[7] ^= v[7] ^ v[15];
}

void blake2_get_impl_list(blake2_impl_list *list)
{
    list->count = 0;
}
