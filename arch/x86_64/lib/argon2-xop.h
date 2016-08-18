#ifndef ARGON2_XOP_H
#define ARGON2_XOP_H

#include "core.h"

int check_xop(void);

void fill_segment_xop(const argon2_instance_t *instance,
                      argon2_position_t position);

#endif // ARGON2_XOP_H
