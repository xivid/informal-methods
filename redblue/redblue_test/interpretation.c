#include <assert.h>

#include "interpretation.h"
#include "structures.h"

size_t
length_S_I(struct stream *S, struct buffer *B) {
    assert(S->colour == Red || S->colour == Blue);

    size_t x = (S->colour == Red ? B->x[Red] : B->x[Blue]);
    return S->N - x;
}

uint64_t
element_S_I(struct stream *S, struct buffer *B, size_t i) {
    assert(S->colour == Red || S->colour == Blue);
    size_t x = (S->colour == Red ? B->x[Red] : B->x[Blue]);
    
    assert(x + i < S->N);

    return S->I.items[x + i];
}

size_t
length_S_IB(struct stream *S, struct buffer *B) {
    /* XXX */
    /* Fill all of these in to define your interpretation. */
    /* XXX */
    size_t ret = 0;
    for (size_t i = B->t_valid; i < B->x[Red] + B->x[Blue]; ++i)
        if (B->items[i % B->N_B].c == S->colour)
            ++ret;
    return ret;
}

uint64_t
element_S_IB(struct stream *S, struct buffer *B, size_t i) {
    /* XXX */
    size_t n = 0;
    bool found = false;
    uint64_t ret = 0;
    for (size_t it = B->t_valid; it < B->x[Red] + B->x[Blue]; ++it)
        if (B->items[it % B->N_B].c == S->colour)
            if (n++ == i) {
                found = true;
                ret = B->items[it % B->N_B].v;
            }
    
    assert(found);
    return ret;
}

size_t
length_S_B(struct stream *S, struct buffer *B) {
    /* XXX */
    size_t ret = 0;
    for (size_t i = B->h[Red] + B->h[Blue]; i < B->t_valid; ++i)
        if (B->items[i % B->N_B].c == S->colour)
            ++ret;
    return ret;
}

uint64_t
element_S_B(struct stream *S, struct buffer *B, size_t i) {
    /* XXX */
    size_t n = 0;
    bool found = false;
    uint64_t ret = 0;
    for (size_t it = B->h[Red] + B->h[Blue]; it < B->t_valid; ++it)
        if (B->items[it % B->N_B].c == S->colour)
            if (n++ == i) {
                found = true;
                ret = B->items[it % B->N_B].v;
            }
    
    assert(found);
    return ret;
}

size_t
length_S_BO(struct stream *S, struct buffer *B) {
    /* XXX */
    return B->h[S->colour] - S->O.y;
}

uint64_t
element_S_BO(struct stream *S, struct buffer *B, size_t i) {
    /* XXX */
    assert(S->O.y + i < B->h[S->colour]);
    
    return S->O.items[S->O.y + i];
}

size_t
length_S_O(struct stream *S, struct buffer *B) {
    return S->O.y;
}

uint64_t
element_S_O(struct stream *S, struct buffer *B, size_t i) {
    assert(i < S->O.y);

    return S->O.items[i];
}
