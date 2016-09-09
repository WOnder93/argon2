#ifndef ARGON2_SSSE3_H
#define ARGON2_SSSE3_H

#include "core.h"

void fill_segment_ssse3(const argon2_instance_t *instance,
                        argon2_position_t position);

#endif // ARGON2_SSSE3_H
