#include <stdlib.h>

#ifdef _POSIX_SOURCE
#include <sys/time.h>

struct timestamp {
    struct timeval time;
};

static inline void timestamp_store(struct timestamp *out)
{
    gettimeofday(&out->time, NULL);
}

static inline double timestamp_span_ms(const struct timestamp *start,
                                       const struct timestamp *end)
{
    double res = 0.0;
    res += (end->time.tv_sec - start->time.tv_sec) * 1000.0;
    res += (end->time.tv_usec - start->time.tv_usec) / 1000.0;
    return res;
}
#else
#include <time.h>

struct timestamp {
    clock_t time;
};

static inline void timestamp_store(struct timestamp *out)
{
    out->time = clock();
}

static inline double timestamp_span_ms(const struct timestamp *start,
                                       const struct timestamp *end)
{
    double res = (end->time - start->time) * 1000;
    return res / CLOCKS_PER_SEC;
}
#endif
