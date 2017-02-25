#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "impl-select.h"

#include "cpu-flags.h"
#include "argon2-sse2.h"
#include "argon2-ssse3.h"
#include "argon2-xop.h"
#include "argon2-avx2.h"
#include "argon2-avx512f.h"

#include <x86intrin.h>

#define rotr64(x, n) _lrotr(x, n)

#include "argon2-template-64.h"

void fill_segment_default(const argon2_instance_t *instance,
                          argon2_position_t position)
{
    fill_segment_64(instance, position);
}

void argon2_get_impl_list(argon2_impl_list *list)
{
    static const argon2_impl IMPLS[] = {
        { "x86_64",     NULL,           fill_segment_default },
        { "SSE2",       check_sse2,     fill_segment_sse2 },
        { "SSSE3",      check_ssse3,    fill_segment_ssse3 },
        { "XOP",        check_xop,      fill_segment_xop },
        { "AVX2",       check_avx2,     fill_segment_avx2 },
        { "AVX-512F",   check_avx512f,  fill_segment_avx512f },
    };

    cpu_flags_get();

    list->count = sizeof(IMPLS) / sizeof(IMPLS[0]);
    list->entries = IMPLS;
}
