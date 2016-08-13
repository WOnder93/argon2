#include <string.h>

#include "blake2/blake2.h"
#include "blake2/blake2-impl.h"
#include "blake2/blake2-common.h"
#include "blake2/blake2-opt.h"

#include "core.h"

static void blake2b_increment_counter(blake2b_state *S, uint64_t inc)
{
    S->t[0] += inc;
    S->t[1] += (S->t[0] < inc);
}

static void blake2b_init_state(blake2b_state *S)
{
    memcpy(S->h, blake2b_IV, sizeof(S->h));
    S->t[1] = S->t[0] = 0;
    S->buflen = 0;
}

void blake2b_init(blake2b_state *S, size_t outlen)
{
    blake2b_init_state(S);
    /* XOR initial state with param block: */
    S->h[0] ^= (uint64_t)outlen | (UINT64_C(1) << 16) | (UINT64_C(1) << 24);
}

void blake2b_update(blake2b_state *S, const void *in, size_t inlen)
{
    const uint8_t *pin = (const uint8_t *)in;

    if (S->buflen + inlen > BLAKE2B_BLOCKBYTES) {
        size_t left = S->buflen;
        size_t fill = BLAKE2B_BLOCKBYTES - left;
        memcpy(&S->buf[left], pin, fill);
        blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
        blake2b_compress(S->h, S->buf, S->t, 0);
        S->buflen = 0;
        inlen -= fill;
        pin += fill;
        /* Avoid buffer copies when possible */
        while (inlen > BLAKE2B_BLOCKBYTES) {
            blake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
            blake2b_compress(S->h, pin, S->t, 0);
            inlen -= BLAKE2B_BLOCKBYTES;
            pin += BLAKE2B_BLOCKBYTES;
        }
    }
    memcpy(&S->buf[S->buflen], pin, inlen);
    S->buflen += inlen;
}

void blake2b_final(blake2b_state *S, void *out, size_t outlen)
{
    uint8_t buffer[BLAKE2B_OUTBYTES] = {0};
    unsigned int i;

    blake2b_increment_counter(S, S->buflen);
    memset(&S->buf[S->buflen], 0, BLAKE2B_BLOCKBYTES - S->buflen); /* Padding */
    blake2b_compress(S->h, S->buf, S->t, UINT64_C(0xFFFFFFFFFFFFFFFF));

    for (i = 0; i < 8; ++i) { /* Output full hash to temp buffer */
        store64(buffer + i * sizeof(uint64_t), S->h[i]);
    }

    memcpy(out, buffer, outlen);
    secure_wipe_memory(buffer, sizeof(buffer));
    secure_wipe_memory(S->buf, sizeof(S->buf));
    secure_wipe_memory(S->h, sizeof(S->h));
}

void blake2b_long(void *out, size_t outlen, const void *in, size_t inlen)
{
    uint8_t *pout = (uint8_t *)out;
    blake2b_state blake_state;
    uint8_t outlen_bytes[sizeof(uint32_t)] = {0};

    store32(outlen_bytes, (uint32_t)outlen);
    if (outlen <= BLAKE2B_OUTBYTES) {
        blake2b_init(&blake_state, outlen);
        blake2b_update(&blake_state, outlen_bytes, sizeof(outlen_bytes));
        blake2b_update(&blake_state, in, inlen);
        blake2b_final(&blake_state, pout, outlen);
    } else {
        uint32_t toproduce;
        uint8_t out_buffer[BLAKE2B_OUTBYTES];

        blake2b_init(&blake_state, BLAKE2B_OUTBYTES);
        blake2b_update(&blake_state, outlen_bytes, sizeof(outlen_bytes));
        blake2b_update(&blake_state, in, inlen);
        blake2b_final(&blake_state, out_buffer, BLAKE2B_OUTBYTES);

        memcpy(pout, out_buffer, BLAKE2B_OUTBYTES / 2);
        pout += BLAKE2B_OUTBYTES / 2;
        toproduce = (uint32_t)outlen - BLAKE2B_OUTBYTES / 2;

        while (toproduce > BLAKE2B_OUTBYTES) {
            blake2b_init(&blake_state, BLAKE2B_OUTBYTES);
            blake2b_update(&blake_state, out_buffer, BLAKE2B_OUTBYTES);
            blake2b_final(&blake_state, out_buffer, BLAKE2B_OUTBYTES);

            memcpy(pout, out_buffer, BLAKE2B_OUTBYTES / 2);
            pout += BLAKE2B_OUTBYTES / 2;
            toproduce -= BLAKE2B_OUTBYTES / 2;
        }

        blake2b_init(&blake_state, toproduce);
        blake2b_update(&blake_state, out_buffer, BLAKE2B_OUTBYTES);
        blake2b_final(&blake_state, out_buffer, toproduce);

        memcpy(pout, out_buffer, toproduce);
    }
}
