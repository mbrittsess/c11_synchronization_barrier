#include "barriers.h"

#include <stdio.h>

void barrier_init ( barrier_t *out, unsigned int num_threads )
{
    out->num_threads_left = out->num_threads = num_threads;
    cnd_init( &out->condition );
    mtx_init( &out->lock, mtx_plain );
};

void barrier_synchronize ( barrier_t *barrier )
{
    cnd_t *condition = &barrier->condition;
    mtx_t *lock      = &barrier->lock;
    unsigned int num_threads_left;
    
    //puts( "Acquiring..." );
    mtx_lock( lock );
        if ( (num_threads_left = --(barrier->num_threads_left)) > 0 )
        {
            //printf( "%d threads left, waiting...\n", num_threads_left );
            cnd_wait( condition, lock );
        }
        else
        {
            //printf( "%d threads left, broadcasting...\n", num_threads_left );
            barrier->num_threads_left = barrier->num_threads;
            cnd_broadcast( condition );
        };
    mtx_unlock( lock );
    
    return;
};
