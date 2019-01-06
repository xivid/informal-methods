#include "atomics.h"
#include "operations.h"

void
push(struct stream *S, struct buffer *B, struct thread_data *td) {
    /* XXX */
    /* Replace this with your implementation. */
    /* XXX */

    /* Keep this here, it records which function the thread is in. */
    STEP_HERE(&S->push_step, td);

    /* This will check the invariants.  Call *only* after a STEP_END, and
     * before a STEP_START (takes a global lock). */
    CHECK(&S->push_step, td);

    STEP_START(&S->push_step, td);
    /* This is how you start a loop. */
    while(!S->I.T) {                                    // 1
        /* This matches the STEP_START, and records that we entered the loop. */
        STEP_END(&S->push_step, td);

        CHECK(&S->push_step, td);

        STEP_START(&S->push_step, td);
        size_t t = B->t_used;                           // 2
        STEP_END(&S->push_step, td);
        CHECK(&S->push_step, td);

        STEP_START(&S->push_step, td);
        if(t == B->t_valid) {                           // 3
            STEP_END(&S->push_step, td);

            CHECK(&S->push_step, td);

            /* If statements with just like while loops. */
            STEP_START(&S->push_step, td);
            if(t - B->h[Red] - B->h[Blue] < B->N_B) {               // 4
                STEP_END(&S->push_step, td);

                CHECK(&S->push_step, td);

                STEP_START(&S->push_step, td);
                S->I.T = CAS(&B->t_used, &t, t+1);      // 5
                STEP_END(&S->push_step, td);
                CHECK(&S->push_step, td);

                STEP_START(&S->push_step, td);
            }
            /* Don't need a STEP_END here: STEP_START is matched outside the
             * outer if. */
        }
        STEP_END(&S->push_step, td);

        CHECK(&S->push_step, td);

        /* This will be matched either by the STEP_END outside the loop, or
         * the one at the start of the loop. */
        STEP_START(&S->push_step, td);
    }
    /* This matches either the STEP_START before the loop (never executed), or
     * the one at the end of the body.  If statements work the same way. */
    STEP_END(&S->push_step, td);

    CHECK(&S->push_step, td);

    /* Every atomic statement should be wrapped by these. This lets the
     * debugger inspect the state and check invariants between statements, and
     * keep track of where we're up to. */
    STEP_START(&S->push_step, td);
    B->items[B->t_valid % B->N_B].v = S->I.items[B->x[S->colour]];   // 6
    STEP_END(&S->push_step, td);
    CHECK(&S->push_step, td);

    STEP_START(&S->push_step, td);
    B->items[B->t_valid % B->N_B].c = S->colour;                    // 7
    STEP_END(&S->push_step, td);
    CHECK(&S->push_step, td);

    STEP_START(&S->push_step, td);
    B->x[S->colour] = B->x[S->colour] + 1;                           // 8
    STEP_END(&S->push_step, td);
    CHECK(&S->push_step, td);

    STEP_START(&S->push_step, td);
    S->I.T = false;                                                 // 9
    STEP_END(&S->push_step, td);
    CHECK(&S->push_step, td);

    STEP_START(&S->push_step, td);
    B->t_valid = B->t_valid + 1;                                    // 10
    STEP_END(&S->push_step, td);
    CHECK(&S->push_step, td);

    STEP_START(&S->push_step, td);
    S->I.x = S->I.x + 1;                                            // 11
    STEP_END(&S->push_step, td);
    CHECK(&S->push_step, td);
}

void
pop(struct stream *S, struct buffer *B, struct thread_data *td) {
    /* XXX */
    /* Replace this with your implementation. */
    /* XXX */

    STEP_HERE(&S->pop_step, td);

    CHECK(&S->pop_step, td);

    STEP_START(&S->pop_step, td);
    while (!S->O.H) {
        STEP_END(&S->pop_step, td);
        CHECK(&S->pop_step, td);

        STEP_START(&S->pop_step, td);
        size_t h = B->h_used;
        STEP_END(&S->pop_step, td);
        CHECK(&S->pop_step, td);

        STEP_START(&S->pop_step, td);
        if (h == B->h[Red] + B->h[Blue]) {
            STEP_END(&S->pop_step, td);
            CHECK(&S->pop_step, td);

            STEP_START(&S->pop_step, td);
            if (h < B->t_valid && B->items[h % B->N_B].c == S->colour) {
                STEP_END(&S->pop_step, td);
                CHECK(&S->pop_step, td);

                STEP_START(&S->pop_step, td);
                S->O.H = CAS(&B->h_used, &h, h+1);
                STEP_END(&S->pop_step, td);
                CHECK(&S->pop_step, td);

                STEP_START(&S->pop_step, td);
            }
        }
        STEP_END(&S->pop_step, td);
        CHECK(&S->pop_step, td);

        STEP_START(&S->pop_step, td);
    }
    STEP_END(&S->pop_step, td);
    CHECK(&S->pop_step, td);

    STEP_START(&S->pop_step, td);
    S->O.items[S->O.y] = B->items[(B->h[Red] + B->h[Blue]) % B->N_B].v;
    STEP_END(&S->pop_step, td);
    CHECK(&S->pop_step, td);

    // STEP_START(&S->pop_step, td);
    // S->O.h = S->O.h + 1;
    // STEP_END(&S->pop_step, td);
    // CHECK(&S->pop_step, td);

    STEP_START(&S->pop_step, td);
    S->O.H = false;
    STEP_END(&S->pop_step, td);
    CHECK(&S->pop_step, td);

    STEP_START(&S->pop_step, td);
    B->h[S->colour] = B->h[S->colour] + 1;
    STEP_END(&S->pop_step, td);
    CHECK(&S->pop_step, td);

    STEP_START(&S->pop_step, td);
    S->O.y = S->O.y + 1;
    STEP_END(&S->pop_step, td);
    CHECK(&S->pop_step, td);
}
