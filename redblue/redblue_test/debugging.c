#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debugging.h"
#include "interpretation.h"
#include "predicates.h"

/*** Interpretation State Inspection ***/

char
inspect_B_colour(struct buffer *B, size_t i) {
    assert ((B->h[Red] + B->h[Blue]) + i < B->t_valid);

    switch (B->items[i].c) {
        case Red:
            return 'R';
        case Blue:
            return 'B';
        default:
            return '?';
    }
}

uint64_t
inspect_B_value(struct buffer *B, size_t i) {
    assert ((B->h[Red] + B->h[Blue]) + i < B->t_valid);
    
    return B->items[i].v;
}

void
inspect_B(struct buffer *B) {
    /* XXX */
    /* Print the contents of your buffer here (depends how you implement it).
     * */
    /* XXX */

    size_t n= B->t_valid - (B->h[Red] + B->h[Blue]);
    
    if(n == 0) {
        printf("[]");
    } else if (n == 1) {
        printf("[%c%"PRIu64"]", inspect_B_colour(B, 0), inspect_B_value(B, 0));
    } else if (n == 2) {
        printf("[%c%"PRIu64",%c%"PRIu64"]",
               inspect_B_colour(B, 0), inspect_B_value(B, 0),
               inspect_B_colour(B, 1), inspect_B_value(B, 1));
    } else {
        printf("[%c%"PRIu64",...,%c%"PRIu64"]",
               inspect_B_colour(B, 0), inspect_B_value(B, 0),
               inspect_B_colour(B, n-1), inspect_B_value(B, n-1));
    }

    printf(" (%zu)", n);
}

void
inspect_substream(struct stream *S, struct buffer *B,
        size_t (*length)(struct stream *, struct buffer *),
        uint64_t (*element)(struct stream *, struct buffer *, size_t)) {
    size_t n= length(S, B);
    
    if(n == 0) {
        printf("[]");
    } else if (n == 1) {
        printf("[%"PRIu64"]", element(S, B, 0));
    } else if (n == 2) {
        printf("[%"PRIu64",%"PRIu64"]",
               element(S, B, 0), element(S, B, 1));
    } else {
        printf("[%"PRIu64",...,%"PRIu64"]",
               element(S, B, 0), element(S, B, n-1));
    }

    printf(" (%zu)", n);
}

void
inspect_S_I(struct stream *S, struct buffer *B) {
    inspect_substream(S, B, length_S_I, element_S_I);
}

void
inspect_S_IB(struct stream *S, struct buffer *B) {
    inspect_substream(S, B, length_S_IB, element_S_IB);
}

void
inspect_S_B(struct stream *S, struct buffer *B) {
    inspect_substream(S, B, length_S_B, element_S_B);
}

void
inspect_S_BO(struct stream *S, struct buffer *B) {
    inspect_substream(S, B, length_S_BO, element_S_BO);
}

void
inspect_S_O(struct stream *S, struct buffer *B) {
    inspect_substream(S, B, length_S_O, element_S_O);
}

#define bool2str(x) ((x)?"true":"false")

void
inspect(struct stream *red, struct stream *blue, struct buffer *B) {
    printf("---\n");
    printf("State Snapshot:\n");

    printf("  N_B: %zu h_used: %zu h[Red]+h[Blue]: %zu t_used: %zu t_valid: %zu\n",
            B->N_B, B->h_used, (B->h[Red] + B->h[Blue]), B->t_used, B->t_valid);
    
    printf("  h[Red]: %zu h[Blue]: %zu x[Red]: %zu x[Blue]: %zu\n",
            B->h[Red], B->h[Blue], B->x[Red], B->x[Blue]);

    printf("  Red.N: %zu Red.I.x: %zu Red.I.T: %s\n",
            red->N, red->I.x, bool2str(red->I.T));

    printf("  Red.O.y: %zu Red.O.H: %s\n",
            red->O.y, bool2str(red->O.H));

    printf("  Blue.N: %zu Blue.I.x: %zu Blue.I.T: %s\n",
            blue->N, blue->I.x, bool2str(blue->I.T));

    printf("  Blue.O.y: %zu Blue.O.H: %s\n",
            blue->O.y, bool2str(blue->O.H));

    printf("  B = ");
    inspect_B(B);
    printf("\n");

    printf("  Red.S_I   = ");
    inspect_S_I(red, B);
    printf("\n");

    printf("  Red.S_IB  = ");
    inspect_S_IB(red, B);
    printf("\n");

    printf("  Red.S_B   = ");
    inspect_S_B(red, B);
    printf("\n");

    printf("  Red.S_BO  = ");
    inspect_S_BO(red, B);
    printf("\n");

    printf("  Red.S_O   = ");
    inspect_S_O(red, B);
    printf("\n");

    printf("  Blue.S_I  = ");
    inspect_S_I(blue, B);
    printf("\n");

    printf("  Blue.S_IB = ");
    inspect_S_IB(blue, B);
    printf("\n");

    printf("  Blue.S_B  = ");
    inspect_S_B(blue, B);
    printf("\n");

    printf("  Blue.S_BO = ");
    inspect_S_BO(blue, B);
    printf("\n");

    printf("  Blue.S_O  = ");
    inspect_S_O(blue, B);
    printf("\n");
    printf("---\n");
}

/*** Invariant Checking ***/

void
check_invariants(struct thread_data *td, const char *func, int line) {
    bool violated= false;

    /* XXX */
    /* If you add an invariant, add it to the checks here. */
    /* XXX */
    if(!Inv_I(td->red, td->B)) {
        violated= true;
        printf("Red.I ");
    }
    if(!Inv_I(td->blue, td->B)) {
        violated= true;
        printf("Blue.I ");
    }
    if(!Inv_O(td->red, td->B)) {
        violated= true;
        printf("Red.O ");
    }
    if(!Inv_O(td->blue, td->B)) {
        violated= true;
        printf("Blue.O ");
    }
    if(!Inv_B(td->B)) {
        violated= true;
        printf("B ");
    }
    if(!Inv_T(td->red, td->blue, td->B)) {
        violated= true;
        printf("T ");
    }
    if(!Inv_S(td->red, td->B)) {
        violated= true;
        printf("Red.S ");
    }
    if(!Inv_S(td->blue, td->B)) {
        violated= true;
        printf("Blue.S ");
    }

    if(violated) {
        printf("violated at %s.%s():%d\n", td->name, func, line);
        inspect(td->red, td->blue, td->B);
        printf("Thread positions:\n");
        thread_positions(td->red, td->blue);
        printf("---\n");
        exit(EXIT_FAILURE);
    }
}

/*** Thread Debugging ***/
void
thread_positions(struct stream *red, struct stream *blue) {
    printf("red_sender.%s():%d\n", red->push_step.function,
                                   red->push_step.line_number);
    printf("red_receiver.%s():%d\n", red->pop_step.function,
                                     red->pop_step.line_number);
    printf("blue_sender.%s():%d\n", blue->push_step.function,
                                    blue->push_step.line_number);
    printf("blue_receiver.%s():%d\n", blue->pop_step.function,
                                      blue->pop_step.line_number);
}
