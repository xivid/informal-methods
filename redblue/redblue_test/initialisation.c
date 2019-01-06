#include "initialisation.h"

void
init_buffer_locals(struct buffer *B, size_t N_B) {
    /* XXX */
    /* Initialise any state you add to the ring buffer here. */
    /* XXX */

    B->h_used = 0;
    B->h[Red] = 0;
    B->h[Blue] = 0;
    B->t_used = 0;
    B->t_valid = 0;
    B->x[Red] = 0;
    B->x[Blue] = 0;
}

void
init_stream_locals(struct stream *S, enum colour colour, size_t N) {
    /* XXX */
    /* Initialise any state you add to the input/output stream here. */
    /* XXX */

    S->I.x = 0;
    S->I.T = false;

    S->O.y = 0;
    S->O.H = false;
}
