#include "argon2-sse2.h"

#ifdef HAVE_SSE2
#include <x86intrin.h>

#define ror64(r, c) \
    _mm_xor_si128(_mm_srli_epi64((r), (c)), _mm_slli_epi64((r), 64 - (c)))

static __m128i f(__m128i x, __m128i y)
{
    __m128i z = _mm_mul_epu32(x, y);
    return _mm_add_epi64(_mm_add_epi64(x, y), _mm_add_epi64(z, z));
}

#define G1(A0, B0, C0, D0, A1, B1, C1, D1) \
    do { \
        A0 = f(A0, B0); \
        A1 = f(A1, B1); \
\
        D0 = _mm_xor_si128(D0, A0); \
        D1 = _mm_xor_si128(D1, A1); \
\
        D0 = ror64(D0, 32); \
        D1 = ror64(D1, 32); \
\
        C0 = f(C0, D0); \
        C1 = f(C1, D1); \
\
        B0 = _mm_xor_si128(B0, C0); \
        B1 = _mm_xor_si128(B1, C1); \
\
        B0 = ror64(B0, 24); \
        B1 = ror64(B1, 24); \
    } while ((void)0, 0)

#define G2(A0, B0, C0, D0, A1, B1, C1, D1) \
    do { \
        A0 = f(A0, B0); \
        A1 = f(A1, B1); \
\
        D0 = _mm_xor_si128(D0, A0); \
        D1 = _mm_xor_si128(D1, A1); \
\
        D0 = ror64(D0, 16); \
        D1 = ror64(D1, 16); \
\
        C0 = f(C0, D0); \
        C1 = f(C1, D1); \
\
        B0 = _mm_xor_si128(B0, C0); \
        B1 = _mm_xor_si128(B1, C1); \
\
        B0 = ror64(B0, 63); \
        B1 = ror64(B1, 63); \
    } while ((void)0, 0)

#define DIAGONALIZE(A0, B0, C0, D0, A1, B1, C1, D1) \
    do { \
        __m128i t0 = D0; \
        __m128i t1 = B0; \
        D0 = _mm_unpackhi_epi64(D1, _mm_unpacklo_epi64(t0, t0)); \
        D1 = _mm_unpackhi_epi64(t0, _mm_unpacklo_epi64(D1, D1)); \
        B0 = _mm_unpackhi_epi64(B0, _mm_unpacklo_epi64(B1, B1)); \
        B1 = _mm_unpackhi_epi64(B1, _mm_unpacklo_epi64(t1, t1)); \
    } while ((void)0, 0)

#define UNDIAGONALIZE(A0, B0, C0, D0, A1, B1, C1, D1) \
    do { \
        __m128i t0 = B0; \
        __m128i t1 = D0; \
        B0 = _mm_unpackhi_epi64(B1, _mm_unpacklo_epi64(B0, B0)); \
        B1 = _mm_unpackhi_epi64(t0, _mm_unpacklo_epi64(B1, B1)); \
        D0 = _mm_unpackhi_epi64(D0, _mm_unpacklo_epi64(D1, D1)); \
        D1 = _mm_unpackhi_epi64(D1, _mm_unpacklo_epi64(t1, t1)); \
    } while ((void)0, 0)

#define BLAKE2_ROUND(A0, A1, B0, B1, C0, C1, D0, D1) \
    do { \
        G1(A0, B0, C0, D0, A1, B1, C1, D1); \
        G2(A0, B0, C0, D0, A1, B1, C1, D1); \
\
        DIAGONALIZE(A0, B0, C0, D0, A1, B1, C1, D1); \
\
        G1(A0, B0, C1, D0, A1, B1, C0, D1); \
        G2(A0, B0, C1, D0, A1, B1, C0, D1); \
\
        UNDIAGONALIZE(A0, B0, C0, D0, A1, B1, C1, D1); \
    } while ((void)0, 0)

#include "argon2-template-128.h"

void fill_segment_sse2(const argon2_instance_t *instance,
                       argon2_position_t position)
{
    fill_segment_128(instance, position);
}

#else

void fill_segment_sse2(const argon2_instance_t *instance,
                       argon2_position_t position)
{
}

#endif
