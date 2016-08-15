#include "argon2-ssse3.h"

#ifdef HAVE_SSSE3
#include <string.h>

#include <cpuid.h>
#include <x86intrin.h>

#define r16 \
    (_mm_setr_epi8(2, 3, 4, 5, 6, 7, 0, 1, 10, 11, 12, 13, 14, 15, 8, 9))
#define r24 \
    (_mm_setr_epi8(3, 4, 5, 6, 7, 0, 1, 2, 11, 12, 13, 14, 15, 8, 9, 10))

#define ror64_16(x) _mm_shuffle_epi8((x), r16)
#define ror64_24(x) _mm_shuffle_epi8((x), r24)
#define ror64_32(x) _mm_shuffle_epi32((x), _MM_SHUFFLE(2, 3, 0, 1))
#define ror64_63(x) \
    _mm_xor_si128(_mm_srli_epi64((x), 63), _mm_add_epi64((x), (x)))

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
        D0 = ror64_32(D0); \
        D1 = ror64_32(D1); \
\
        C0 = f(C0, D0); \
        C1 = f(C1, D1); \
\
        B0 = _mm_xor_si128(B0, C0); \
        B1 = _mm_xor_si128(B1, C1); \
\
        B0 = ror64_24(B0); \
        B1 = ror64_24(B1); \
    } while ((void)0, 0)

#define G2(A0, B0, C0, D0, A1, B1, C1, D1) \
    do { \
        A0 = f(A0, B0); \
        A1 = f(A1, B1); \
\
        D0 = _mm_xor_si128(D0, A0); \
        D1 = _mm_xor_si128(D1, A1); \
\
        D0 = ror64_16(D0); \
        D1 = ror64_16(D1); \
\
        C0 = f(C0, D0); \
        C1 = f(C1, D1); \
\
        B0 = _mm_xor_si128(B0, C0); \
        B1 = _mm_xor_si128(B1, C1); \
\
        B0 = ror64_63(B0); \
        B1 = ror64_63(B1); \
    } while ((void)0, 0)

#define DIAGONALIZE(A0, B0, C0, D0, A1, B1, C1, D1) \
    do { \
        __m128i t0 = _mm_alignr_epi8(B1, B0, 8); \
        __m128i t1 = _mm_alignr_epi8(B0, B1, 8); \
        B0 = t0; \
        B1 = t1; \
\
        t0 = C0; \
        C0 = C1; \
        C1 = t0; \
\
        t0 = _mm_alignr_epi8(D1, D0, 8); \
        t1 = _mm_alignr_epi8(D0, D1, 8); \
        D0 = t1; \
        D1 = t0; \
    } while ((void)0, 0)

#define UNDIAGONALIZE(A0, B0, C0, D0, A1, B1, C1, D1) \
    do { \
        __m128i t0 = _mm_alignr_epi8(B0, B1, 8); \
        __m128i t1 = _mm_alignr_epi8(B1, B0, 8); \
        B0 = t0; \
        B1 = t1; \
\
        t0 = C0; \
        C0 = C1; \
        C1 = t0; \
\
        t0 = _mm_alignr_epi8(D0, D1, 8); \
        t1 = _mm_alignr_epi8(D1, D0, 8); \
        D0 = t1; \
        D1 = t0; \
    } while ((void)0, 0)

#define BLAKE2_ROUND(A0, A1, B0, B1, C0, C1, D0, D1) \
    do { \
        G1(A0, B0, C0, D0, A1, B1, C1, D1); \
        G2(A0, B0, C0, D0, A1, B1, C1, D1); \
\
        DIAGONALIZE(A0, B0, C0, D0, A1, B1, C1, D1); \
\
        G1(A0, B0, C0, D0, A1, B1, C1, D1); \
        G2(A0, B0, C0, D0, A1, B1, C1, D1); \
\
        UNDIAGONALIZE(A0, B0, C0, D0, A1, B1, C1, D1); \
    } while ((void)0, 0)

static void fill_block(__m128i *s, const block *ref_block, block *next_block,
                       int with_xor)
{
    __m128i block_XY[ARGON2_OWORDS_IN_BLOCK];
    unsigned int i;

    if (with_xor) {
        for (i = 0; i < ARGON2_OWORDS_IN_BLOCK; i++) {
            s[i] =_mm_xor_si128(
                s[i], _mm_loadu_si128((const __m128i *)ref_block->v + i));
            block_XY[i] = _mm_xor_si128(
                s[i], _mm_loadu_si128((const __m128i *)next_block->v + i));
        }

    } else {
        for (i = 0; i < ARGON2_OWORDS_IN_BLOCK; i++) {
            block_XY[i] = s[i] =_mm_xor_si128(
                s[i], _mm_loadu_si128((const __m128i *)ref_block->v + i));
        }
    }

    for (i = 0; i < 8; ++i) {
        BLAKE2_ROUND(
            s[8 * i + 0], s[8 * i + 1], s[8 * i + 2], s[8 * i + 3],
            s[8 * i + 4], s[8 * i + 5], s[8 * i + 6], s[8 * i + 7]);
    }

    for (i = 0; i < 8; ++i) {
        BLAKE2_ROUND(
            s[8 * 0 + i], s[8 * 1 + i], s[8 * 2 + i], s[8 * 3 + i],
            s[8 * 4 + i], s[8 * 5 + i], s[8 * 6 + i], s[8 * 7 + i]);
    }

    for (i = 0; i < ARGON2_OWORDS_IN_BLOCK; i++) {
        s[i] = _mm_xor_si128(s[i], block_XY[i]);
        _mm_storeu_si128((__m128i *)next_block->v + i, s[i]);
    }
}

static void generate_addresses(const argon2_instance_t *instance,
                               const argon2_position_t *position,
                               uint64_t *pseudo_rands)
{
    block address_block, input_block, tmp_block;
    unsigned int i;

    init_block_value(&address_block, 0);
    init_block_value(&input_block, 0);

    if (instance != NULL && position != NULL) {
        input_block.v[0] = position->pass;
        input_block.v[1] = position->lane;
        input_block.v[2] = position->slice;
        input_block.v[3] = instance->memory_blocks;
        input_block.v[4] = instance->passes;
        input_block.v[5] = instance->type;

        for (i = 0; i < instance->segment_length; ++i) {
            if (i % ARGON2_ADDRESSES_IN_BLOCK == 0) {
                /*Temporary zero-initialized blocks*/
                __m128i zero_block[ARGON2_OWORDS_IN_BLOCK];
                __m128i zero2_block[ARGON2_OWORDS_IN_BLOCK];

                memset(zero_block, 0, sizeof(zero_block));
                memset(zero2_block, 0, sizeof(zero2_block));

                init_block_value(&address_block, 0);
                init_block_value(&tmp_block, 0);

                /*Increasing index counter*/
                input_block.v[6]++;

                /*First iteration of G*/
                fill_block(zero_block, &input_block, &tmp_block, 1);

                /*Second iteration of G*/
                fill_block(zero2_block, &tmp_block, &address_block, 1);
            }

            pseudo_rands[i] = address_block.v[i % ARGON2_ADDRESSES_IN_BLOCK];
        }
    }
}

void fill_segment_ssse3(const argon2_instance_t *instance,
                        argon2_position_t position)
{
    block *ref_block = NULL, *curr_block = NULL;
    uint64_t pseudo_rand, ref_index, ref_lane;
    uint32_t prev_offset, curr_offset;
    uint32_t starting_index, i;
    __m128i state[ARGON2_OWORDS_IN_BLOCK];
    int data_independent_addressing;

    /* Pseudo-random values that determine the reference block position */
    uint64_t *pseudo_rands = NULL;

    if (instance == NULL) {
        return;
    }

    data_independent_addressing = (instance->type == Argon2_i);

    pseudo_rands =
        (uint64_t *)malloc(sizeof(uint64_t) * instance->segment_length);
    if (pseudo_rands == NULL) {
        return;
    }

    if (data_independent_addressing) {
        generate_addresses(instance, &position, pseudo_rands);
    }

    starting_index = 0;

    if ((0 == position.pass) && (0 == position.slice)) {
        starting_index = 2; /* we have already generated the first two blocks */
    }

    /* Offset of the current block */
    curr_offset = position.lane * instance->lane_length +
                  position.slice * instance->segment_length + starting_index;

    if (0 == curr_offset % instance->lane_length) {
        /* Last block in this lane */
        prev_offset = curr_offset + instance->lane_length - 1;
    } else {
        /* Previous block */
        prev_offset = curr_offset - 1;
    }

    memcpy(state, ((instance->memory + prev_offset)->v), ARGON2_BLOCK_SIZE);

    for (i = starting_index; i < instance->segment_length;
         ++i, ++curr_offset, ++prev_offset) {
        /*1.1 Rotating prev_offset if needed */
        if (curr_offset % instance->lane_length == 1) {
            prev_offset = curr_offset - 1;
        }

        /* 1.2 Computing the index of the reference block */
        /* 1.2.1 Taking pseudo-random value from the previous block */
        if (data_independent_addressing) {
            pseudo_rand = pseudo_rands[i];
        } else {
            pseudo_rand = instance->memory[prev_offset].v[0];
        }

        /* 1.2.2 Computing the lane of the reference block */
        ref_lane = ((pseudo_rand >> 32)) % instance->lanes;

        if ((position.pass == 0) && (position.slice == 0)) {
            /* Can not reference other lanes yet */
            ref_lane = position.lane;
        }

        /* 1.2.3 Computing the number of possible reference block within the
         * lane.
         */
        position.index = i;
        ref_index = index_alpha(instance, &position, pseudo_rand & 0xFFFFFFFF,
                                ref_lane == position.lane);

        /* 2 Creating a new block */
        ref_block =
            instance->memory + instance->lane_length * ref_lane + ref_index;
        curr_block = instance->memory + curr_offset;

        /* version 1.2.1 and earlier: overwrite, not XOR */
        if (0 == position.pass || ARGON2_VERSION_10 == instance->version) {
            fill_block(state, ref_block, curr_block, 0);
        } else {
            fill_block(state, ref_block, curr_block, 1);
        }
    }

    free(pseudo_rands);
}

int check_ssse3(void)
{
    /* Check if SSSE3 instructions are supported: */
    unsigned int info[4];
    if (!__get_cpuid(0x00000001, &info[0], &info[1], &info[2], &info[3])) {
        return 0;
    }
    // FIXME: check also OS support!
    return (info[3] & bit_SSE2) != 0 &&
            (info[2] & bit_SSE3) != 0 &&
            (info[2] & bit_SSSE3) != 0;
}

#else

void fill_segment_ssse3(const argon2_instance_t *instance,
                        argon2_position_t position)
{
}

int check_ssse3(void) { return 0; }

#endif
