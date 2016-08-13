#include "impl-select.h"

#include "argon2-opt.h"
#include "blake2/blake2-opt.h"

static argon2_impl selected_argon_impl = {
    "(default)", fill_segment_default
};

static void select_impl(FILE *out)
{
    argon2_impl_list impls;

    fprintf(out, "Selecting fill_segment function implementation...\n");
    argon2_get_impl_list(&impls);
    if (impls.count == 0) {
        fprintf(out, "  No implementation available, keeping the default!\n");
    } else if (impls.count == 1) {
        fprintf(out, "  Selecting the only available implementation '%s'.\n",
                impls.entries[0].name);
        selected_argon_impl = impls.entries[0];
    } else {
        // TODO
    }
}

void fill_segment(const argon2_instance_t *instance, argon2_position_t position)
{
    selected_argon_impl.fill_segment(instance, position);
}

blake2_impl selected_blake2_impl = {
    "(default)", blake2_update_block_default
};

static void blake2_select_impl(FILE *out)
{
    blake2_impl_list impls;

    fprintf(out, "Selecting BLAKE2 compress function implementation...\n");
    blake2_get_impl_list(&impls);
    if (impls.count == 0) {
        fprintf(out, "  No implementation available, keeping the default!\n");
    } else if (impls.count == 1) {
        fprintf(out, "  Selecting the only available implementation '%s'.\n",
                impls.entries[0].name);
        selected_blake2_impl = impls.entries[0];
    } else {
        // TODO
    }
}

void argon2_select_impl(FILE *out)
{
    blake2_select_impl(out);
    select_impl(out);
}
