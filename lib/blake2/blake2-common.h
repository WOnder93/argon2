#ifndef ARGON2_BLAKE2_COMMON_H
#define ARGON2_BLAKE2_COMMON_H

#include <stdint.h>

static const uint64_t blake2b_IV[8] = {
    UINT64_C(0x6a09e667f3bcc908), UINT64_C(0xbb67ae8584caa73b),
    UINT64_C(0x3c6ef372fe94f82b), UINT64_C(0xa54ff53a5f1d36f1),
    UINT64_C(0x510e527fade682d1), UINT64_C(0x9b05688c2b3e6c1f),
    UINT64_C(0x1f83d9abfb41bd6b), UINT64_C(0x5be0cd19137e2179)
};

#define rotr64(x, n) (((x) >> (n)) | ((x) << (64 - (n))))

#endif // ARGON2_BLAKE2_COMMON_H

