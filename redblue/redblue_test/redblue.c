#define _GNU_SOURCE

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "atomics.h"
#include "debugging.h"
#include "initialisation.h"
#include "interpretation.h"
#include "operations.h"
#include "predicates.h"
#include "structures.h"

pthread_mutex_t big_lock= PTHREAD_MUTEX_INITIALIZER;

bool do_check_invariants;

/* Sender/Receiver Threads */

void *
sender(void *data) {
    struct thread_data *td= (struct thread_data *)data;
    struct buffer *B= td->B;
    struct stream *S;

    if(td->my_colour == Red) S= td->red;
    else                     S= td->blue;

    STORE(&td->running, true);

    STEP_HERE(&S->push_step, td);

    CHECK(&S->push_step, td);

    STEP_START(&S->push_step, td);
    while(S->I.x < S->N) {
        STEP_END(&S->push_step, td);

        push(S, B, td);

        STEP_HERE(&S->push_step, td);

        STEP_START(&S->push_step, td);
    }
    STEP_END(&S->push_step, td);

    CHECK(&S->push_step, td);

    STORE(&td->finished, true);

    return NULL;
}

void *
receiver(void *data) {
    struct thread_data *td= (struct thread_data *)data;
    struct buffer *B= td->B;
    struct stream *S;

    if(td->my_colour == Red) S= td->red;
    else                     S= td->blue;

    STORE(&td->running, true);

    STEP_HERE(&S->pop_step, td);

    CHECK(&S->pop_step, td);

    STEP_START(&S->pop_step, td);
    while(S->O.y < S->N) {
        STEP_END(&S->pop_step, td);

        pop(S, B, td);

        STEP_HERE(&S->pop_step, td);

        STEP_START(&S->pop_step, td);
    }
    STEP_END(&S->pop_step, td);

    CHECK(&S->pop_step, td);

    STORE(&td->finished, true);

    return NULL;
}

/* Initialisation */
void
init_buffer(struct buffer *B, size_t N_B) {
    B->N_B= N_B;
    init_buffer_locals(B, N_B);
    B->items= calloc(N_B, sizeof(struct coloured_item));
    if(!B->items) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
}

void
init_stream(struct stream *S, enum colour colour, size_t N) {
    S->N= N;
    S->colour= colour;
    S->push_step.function= "none";
    S->push_step.line_number= -1;
    S->I.items= calloc(N, sizeof(uint64_t));
    if(!S->I.items) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    S->pop_step.function= "none";
    S->pop_step.line_number= -1;
    S->O.items= calloc(N, sizeof(uint64_t));
    if(!S->O.items) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    init_stream_locals(S, colour, N);
}

void
create_thread(pthread_t *thread, void *(*start)(void *),
              struct thread_data *td, enum colour c, const char *name,
              struct stream *red, struct stream *blue, struct buffer *B) {
    td->my_colour= c;
    td->name= name;
    td->running= false;
    td->finished= false;
    td->red= red;
    td->blue= blue;
    td->B= B;

    if(pthread_create(thread, NULL, start, td)) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
}

/* Entry Point */

#define DEFAULT_N 100
#define DEFAULT_N_B 10

void
usage(const char *name) {
    printf("Usage: %s [-n <stream_length>] [-b <buffer_size>] [-c] [-h]\n"
           "  -c : Check invariants at every interference point (slow)\n"
           "       (disables periodic status printouts).\n"
           "  -n : Change stream length (default %d)\n"
           "  -b : Change buffer size (default %d)\n"
           "  -h : Show this documentation.\n",
           name, DEFAULT_N, DEFAULT_N_B);
    exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[]) {
    struct stream red, blue;
    struct buffer B;
    pthread_t red_sender, red_receiver;
    pthread_t blue_sender, blue_receiver;
    struct thread_data td_red_sender, td_red_receiver;
    struct thread_data td_blue_sender, td_blue_receiver;
    size_t N, N_B;

    {
        int opt;

        do_check_invariants= false;
        N= DEFAULT_N;
        N_B= DEFAULT_N_B;

        while((opt = getopt(argc, argv, "chn:b:")) != -1) {
            switch(opt) {
                case 'c':
                    do_check_invariants= true;
                    break;
                case 'n':
                    errno= 0;
                    N= strtoul(optarg, NULL, 10);
                    if(errno) {
                        perror("strtoul");
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'b':
                    errno= 0;
                    N_B= strtoul(optarg, NULL, 10);
                    if(errno) {
                        perror("strtoul");
                        exit(EXIT_FAILURE);
                    }
                    break;
                default:
                    usage(argv[0]);
                    break;
            }
        }
    }

    printf("Streams: %zu items each, Bounded Buffer: %zu items\n", N, N_B);

    init_buffer(&B, N_B);
    init_stream(&red, Red, N);
    init_stream(&blue, Blue, N);

    printf("Generating stream data.\n");

    /* The red stream gets even numbers. */
    for(uint64_t i= 0; i < red.N; i++) {
        red.I.items[i]= 2 * i;
    }

    /* The blue stream gets odd numbers. */
    for(uint64_t i= 0; i < blue.N; i++) {
        blue.I.items[i]= 2 * i + 1;
    }

    inspect(&red, &blue, &B);

    printf("Creating sender and receiver threads.\n");

    /* The threads will block on the big lock until we release them. */
    pthread_mutex_lock(&big_lock);

    create_thread(&red_sender, sender, &td_red_sender, Red,
                  "red_sender", &red, &blue, &B);
    create_thread(&red_receiver, receiver, &td_red_receiver, Red,
                  "red_receiver", &red, &blue, &B);
    create_thread(&blue_sender, sender, &td_blue_sender, Blue,
                  "blue_sender", &red, &blue, &B);
    create_thread(&blue_receiver, receiver, &td_blue_receiver, Blue,
                  "blue_receiver", &red, &blue, &B);

    printf("Waiting for threads to start.\n");

    while(!LOAD(&td_red_sender.running));
    while(!LOAD(&td_red_receiver.running));
    while(!LOAD(&td_blue_sender.running));
    while(!LOAD(&td_blue_receiver.running));

    printf("Naming threads.\n");

    if(pthread_setname_np(red_receiver, td_red_receiver.name)) {
        perror("pthread_setname_np");
        exit(EXIT_FAILURE);
    }

    if(pthread_setname_np(red_sender, td_red_sender.name)) {
        perror("pthread_setname_np");
        exit(EXIT_FAILURE);
    }

    if(pthread_setname_np(blue_receiver, td_blue_receiver.name)) {
        perror("pthread_setname_np");
        exit(EXIT_FAILURE);
    }

    if(pthread_setname_np(blue_sender, td_blue_sender.name)) {
        perror("pthread_setname_np");
        exit(EXIT_FAILURE);
    }

    printf("Unblocking threads.\n");

    pthread_mutex_unlock(&big_lock);

    printf("Waiting for senders and receivers to terminate.\n");

    struct thread_data main_thread;
    main_thread.my_colour= Black;
    main_thread.name= "main";
    main_thread.red= &red;
    main_thread.blue= &blue;
    main_thread.B= &B;

    int sleeps= 0;
    while(!LOAD(&td_red_sender.finished) ||
          !LOAD(&td_red_receiver.finished) ||
          !LOAD(&td_blue_sender.finished) ||
          !LOAD(&td_blue_receiver.finished)) {
        usleep(10000);
        sleeps++;
        if(sleeps % 100 == 0) {
            pthread_mutex_lock(&big_lock);
            check_invariants(&main_thread, __func__, __LINE__);
            inspect(&red, &blue, &B);
            printf("Thread positions:\n");
            thread_positions(&red, &blue);
            printf("---\n");
            pthread_mutex_unlock(&big_lock);
        }
    }

    if(pthread_join(red_sender, NULL)) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(red_receiver, NULL)) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(blue_sender, NULL)) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(blue_receiver, NULL)) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    printf("Final system state:\n");
    inspect(&red, &blue, &B);

    if(Postcondition(&red)) {
        printf("Red postcondition satisfied: "
               "Red.O[0,%zu) == Red.I[0,%zu)\n",
               red.N, red.N);
    } else {
        printf("Red postcondition violated: "
               "Red.O[0,%zu) != Red.I[0,%zu)\n",
               red.N, red.N);
    }

    if(Postcondition(&blue)) {
        printf("Blue postcondition satisfied: "
               "Blue.O[0,%zu) == Blue.I[0,%zu)\n",
               blue.N, blue.N);
    } else {
        printf("Blue postcondition violated: "
               "Blue.O[0,%zu) != Blue.I[0,%zu)\n",
               blue.N, blue.N);
    }

    return EXIT_SUCCESS;
}
