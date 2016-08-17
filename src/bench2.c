#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifndef _POSIX_SOURCE
#error "This program will only compile on POSIX systems!"
#endif

#include "argon2.h"
#include "argon2-opt.h"

#include "timing.h"

#define BENCH_OUTLEN 16
#define BENCH_INLEN 16
#define BENCH_SAMPLES 16

static double min(const double *samples, size_t count)
{
    size_t i;
    double min = INFINITY;
    for (i = 0; i < count; i++) {
        if (samples[i] < min) {
            min = samples[i];
        }
    }
    return min;
}

static int benchmark(uint32_t t_cost, uint32_t m_cost, uint32_t p)
{
    static const unsigned char PASSWORD[BENCH_OUTLEN] = { 0 };
    static const unsigned char SALT[BENCH_INLEN] = { 1 };

    unsigned char out[BENCH_OUTLEN];
    struct timestamp start, end;
    double ms_d[BENCH_SAMPLES];
    double ms_i[BENCH_SAMPLES];

    double ms_d_final, ms_i_final;
    unsigned int i;

    int res;

    for (i = 0; i < BENCH_SAMPLES; i++) {
        timestamp_store(&start);
        res = argon2d_hash_raw(t_cost, m_cost, p, PASSWORD, sizeof(PASSWORD),
                               SALT, sizeof(SALT), out, sizeof(out));
        timestamp_store(&end);
        if (res != ARGON2_OK) {
            return res;
        }

        ms_d[i] = timestamp_span_ms(&start, &end);
    }

    for (i = 0; i < BENCH_SAMPLES; i++) {
        timestamp_store(&start);
        res = argon2i_hash_raw(t_cost, m_cost, p, PASSWORD, sizeof(PASSWORD),
                               SALT, sizeof(SALT), out, sizeof(out));
        timestamp_store(&end);
        if (res != ARGON2_OK) {
            return res;
        }

        ms_i[i] = timestamp_span_ms(&start, &end);
    }

    ms_d_final = min(ms_d, BENCH_SAMPLES);
    ms_i_final = min(ms_i, BENCH_SAMPLES);

    printf("%8lu%16lu%8lu%16.3lf%16.3lf\n",
           (unsigned long)t_cost, (unsigned long)m_cost, (unsigned long)p,
           ms_d_final, ms_i_final);
    return 0;
}

int main(void)
{
    uint32_t t_cost, m_cost, p;
    int res;

    argon2_select_impl(stderr);

    printf("%8s%16s%8s%16s%16s\n", "t_cost", "m_cost", "threads",
           "Argon2d (ms)", "Argon2i (ms)");

    for (t_cost = 1; t_cost <= 8; t_cost *= 2) {
        for (m_cost = 1024; m_cost <= 1024 * 1024; m_cost *= 4) {
            for (p = 1; p <= 4; p *= 2) {
                res = benchmark(t_cost, m_cost, p);
                if (res != 0) {
                    return res;
                }
            }
        }
    }
    return 0;
}
