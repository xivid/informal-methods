#include "interpretation.h"
#include "predicates.h"

bool
Inv_I(struct stream *S, struct buffer *B) {
    /* XXX */
    /* Define your invariants here. */
    /* XXX */
    return S->I.x <= S->N;
}

bool
Inv_T(struct stream *red, struct stream *blue, struct buffer *B) {
    /* XXX */
    /* Define your invariants here. */
    /* XXX */
    bool ok = true;
    ok &= !(red->I.T && blue->I.T);
    ok &= !(red->O.H && blue->O.H);
    ok &= (B->h_used - (B->h[Red] + B->h[Blue]) <= 1);
    ok &= (B->t_used - B->t_valid <= 1);
    return ok;
}

bool
Inv_B(struct buffer *B) {
    /* XXX */
    /* Define your invariants here. */
    /* XXX */
    return ((B->h[Red] + B->h[Blue] <= B->t_valid) && (B->t_valid - B->h[Red] - B->h[Blue] <= B->N_B));
}

bool
Inv_O(struct stream *S, struct buffer *B) {
    /* XXX */
    /* Define your invariants here. */
    /* XXX */
    return S->O.y <= S->N;
}

bool
Inv_S(struct stream *S, struct buffer *B) {
    size_t n_I=  length_S_I(S, B);
    size_t n_IB= length_S_IB(S, B);
    size_t n_B=  length_S_B(S, B);
    size_t n_BO= length_S_BO(S, B);
    size_t n_O=  length_S_O(S, B);

    if(n_I + n_IB + n_B + n_BO + n_O != S->N) return false;

    bool ok= true;
    size_t i= 0;

    for(size_t j= 0; j < n_O; i++, j++) {
        ok&= (S->I.items[i] == element_S_O(S, B, j));
    }

    for(size_t j= 0; j < n_BO; i++, j++) {
        ok&= (S->I.items[i] == element_S_BO(S, B, j));
    }

    for(size_t j= 0; j < n_B; i++, j++) {
        ok&= (S->I.items[i] == element_S_B(S, B, j));
    }

    for(size_t j= 0; j < n_IB; i++, j++) {
        ok&= (S->I.items[i] == element_S_IB(S, B, j));
    }

    for(size_t j= 0; j < n_I; i++, j++) {
        ok&= (S->I.items[i] == element_S_I(S, B, j));
    }

    return ok;
}

bool
Postcondition(struct stream *S) {
    bool ok= true;

    for(size_t i= 0; i < S->N; i++) {
        ok&= (S->I.items[i] == S->O.items[i]);
    }

    return ok;
}
