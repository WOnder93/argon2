#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "argon2.h"

#define BENCH_OUTLEN 16
#define BENCH_INLEN 16

static int benchmark(uint32_t t_cost, uint32_t m_cost, uint32_t p)
{
    static const unsigned char PASSWORD[BENCH_OUTLEN] = { 0 };
    static const unsigned char SALT[BENCH_INLEN] = { 1 };

    unsigned char out[BENCH_OUTLEN];
    clock_t start, delta_d, delta_i;
    double ms_d, ms_i;

    int res;

    start = clock();
    res = argon2d_hash_raw(t_cost, m_cost, p, PASSWORD, sizeof(PASSWORD),
                           SALT, sizeof(SALT), out, sizeof(out));
    delta_d = clock() - start;

    if (res != ARGON2_OK) {
        return res;
    }

    start = clock();
    res = argon2i_hash_raw(t_cost, m_cost, p, PASSWORD, sizeof(PASSWORD),
                           SALT, sizeof(SALT), out, sizeof(out));
    delta_i = clock() - start;

    if (res != ARGON2_OK) {
        return res;
    }

    ms_d = (double)(1000 * delta_d) / CLOCKS_PER_SEC;
    ms_i = (double)(1000 * delta_i) / CLOCKS_PER_SEC;

    printf("%8lu%16lu%8lu%16.3lf%16.3lf\n",
           (unsigned long)t_cost, (unsigned long)m_cost, (unsigned long)p,
           ms_d, ms_i);
    return 0;
}

int main(void) {
    uint32_t t_cost, m_cost, p;
    int res;

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
