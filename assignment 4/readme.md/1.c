#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define N 100000
#define M 100003
#define MAXL 32

char reg[N][MAXL];
unsigned int count[3][M];

uint32_t polynomialHash(char *s)
{
    uint32_t h = 0;

    while (*s)
        h = (h * 31u + (unsigned char)*s++) % M;

    return h;
}

uint64_t numericKey(char *s)
{
    uint64_t k = 0;

    while (*s)
    {
        if (*s >= '0' && *s <= '9')
            k = k * 10 + (*s - '0');

        s++;
    }

    return k;
}

uint32_t multiplicationHash(char *s)
{
    const long double A = 0.6180339887498948482L;

    uint64_t k = numericKey(s);
    long double x = k * A;
    long double frac = x - floorl(x);

    return (uint32_t)floorl(M * frac);
}

uint32_t fnv1aHash(char *s)
{
    uint32_t h = 2166136261u;

    while (*s)
    {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }

    return h % M;
}

double elapsed(struct timespec a, struct timespec b)
{
    return (b.tv_sec - a.tv_sec) +
           (b.tv_nsec - a.tv_nsec) / 1e9;
}

int main()
{
    struct timespec start, end;
    double timeTaken[3];
    volatile uint32_t sink = 0;

    int i, j;

    // Generate 100,000 registrations
    for (i = 0; i < N; i++)
        snprintf(reg[i], MAXL, "REG%06d", i);

    // Polynomial rolling hash
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < N; i++)
    {
        uint32_t h = polynomialHash(reg[i]);
        count[0][h]++;
        sink ^= h;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    timeTaken[0] = elapsed(start, end);

    // Multiplication hash
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < N; i++)
    {
        uint32_t h = multiplicationHash(reg[i]);
        count[1][h]++;
        sink ^= h;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    timeTaken[1] = elapsed(start, end);

    // FNV-1a hash
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < N; i++)
    {
        uint32_t h = fnv1aHash(reg[i]);
        count[2][h]++;
        sink ^= h;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    timeTaken[2] = elapsed(start, end);

    char *names[] = {
        "Polynomial Rolling",
        "Multiplication",
        "FNV-1a"
    };

    // Calculate longest chain
    for (j = 0; j < 3; j++)
    {
        unsigned int maxChain = 0;
        unsigned int occupied = 0;

        for (i = 0; i < M; i++)
        {
            if (count[j][i] > 0)
                occupied++;

            if (count[j][i] > maxChain)
                maxChain = count[j][i];
        }

        printf("\n%s\n", names[j]);
        printf("Longest chain: %u\n", maxChain);
        printf("Occupied buckets: %u\n", occupied);
        printf("Execution time: %.6f seconds\n",
               timeTaken[j]);
    }

    return 0;
}
