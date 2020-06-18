#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <threads.h>
#include <stdio.h>
#define atomic _Atomic

#include "barriers.h"

#define num_threads 8
#define num_iterations 5

static volatile atomic int num_threads_finished = 0;
static volatile atomic int current_position = 0;
static int array[ num_threads * num_iterations ];

int worker ( void *arg )
{
    barrier_t *pbarrier = arg;
    int iteration = 1;
    
    do
    {
        array[ atomic_fetch_add( &current_position, 1 ) ] = iteration;
        barrier_synchronize( pbarrier );
    } while ( iteration++ < num_iterations );
    
    atomic_fetch_add( &num_threads_finished, 1 );
    
    return 0;
};

int main ( int argc, char *argv[] )
{
    //puts( "Creating barrier" );
    barrier_t barrier; barrier_init( &barrier, num_threads );
    
    //puts( "Creating threads...");
    for ( int i = 1; i <= num_threads; i++ )
    {
        thrd_create( &(thrd_t){0}, worker, &barrier );
        //puts( "Created a thread..." );
    };
    //puts( "Threads are spawned" );
    
    while ( num_threads_finished != num_threads )
        thrd_yield( );
    
    for ( int i = 0; i < (num_threads * num_iterations); i++ )
        printf( "array[%d] was visited by a thread in iteration %d\n", i, array[i] );
    
    return EXIT_SUCCESS;
};
