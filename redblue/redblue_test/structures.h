#ifndef __STRUCTURES_H
#define __STRUCTURES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

enum colour {
    Black = 0, /* Item never written (initialisation default). */
    Red   = 1, /* Item last written by red. */
    Blue  = 2  /* Item list written by blue. */
};

#ifdef BUILD_REF
#include "structures_ref.h"
#else

/*** The Circular Buffer ***/

/* XXX - Start Editing Here */

/* Items in the circular buffer are tagged with a colour, to tell them
 * apart. */
struct coloured_item {
    uint64_t v;
    enum colour c;
};

struct buffer {
    /* Buffer size (constant) */
    size_t N_B;

    /* XXX */
    /* Change this when you add or remove buffer variables. */
    /* XXX */
    
    /* Head and tail pointers (shared variables) */
    size_t h_used, t_used, t_valid;

    /* some pointers of two streams are moved into buffer to help interpretation */
    size_t x[3];  // x[1] is x_red, x[2] is x_blue;
    size_t h[3];  // h[1] is h_red, h[2] is h_blue;  

    /* The buffer's contents (shared variable) */
    struct coloured_item *items;
};

/*** An Input Stream ***/

struct input_state {
    /* Input stream private state (variables, private to xxx_sender) */
    size_t x;

    /* XXX */
    /* Change this when you add or remove input stream variables. */
    /* XXX */
    bool T;     // owns the tail pointer

    /* The input items (constant once initialised) */
    uint64_t *items;
};

/*** An Output Stream ***/

struct output_state {
    /* Output stream private state (variables, private to xxx_receiver) */
    size_t y;

    /* XXX */
    /* Change this when you add or remove output stream variables. */
    /* XXX */
    bool H;     // owns the head pointer

    /* The output items (variable, private to xxx_receiver) */
    uint64_t *items;
};

/* XXX - Stop Editing Here */

#endif

/*** A Coloured Stream (minus circular buffer state) ***/

/* Tracks the position of a thread for debugging. */
struct step_counter {
    const char *function;
    int line_number;
};

struct stream {
    /* Per-stream constants */
    size_t N;
    enum colour colour;

    /* Private state for xxx_sender */
    struct step_counter push_step;
    struct input_state I;

    /* Private state for the xxx_receiver */
    struct step_counter pop_step;
    struct output_state O;
};

/*** Per-Thread State ***/
struct thread_data {
    enum colour my_colour;
    const char *name;
    bool running, finished;

    /* Pointers to all system state. */
    struct stream *red, *blue;
    struct buffer *B;
};

#endif /* __STRUCTURES_H */
