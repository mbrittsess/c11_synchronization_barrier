#pragma once

#include <threads.h>

typedef struct
{
    unsigned int num_threads;
    unsigned int num_threads_left;
    cnd_t condition;
    mtx_t lock;
} barrier_t;

void barrier_init ( barrier_t *out, unsigned int num_threads );
void barrier_synchronize ( barrier_t *barrier );
