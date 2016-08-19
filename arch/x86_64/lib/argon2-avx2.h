#ifndef ARGON2_AVX2_H
#define ARGON2_AVX2_H

#include "core.h"

int check_avx2(void);

void fill_segment_avx2(const argon2_instance_t *instance,
                       argon2_position_t position);

#endif // ARGON2_AVX2_H
