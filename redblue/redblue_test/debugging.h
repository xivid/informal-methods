#ifndef __DEBUGGING_H
#define __DEBUGGING_H

#include "structures.h"

static inline const char *
colour_name(enum colour c) {
    switch(c) {
        case Black: return "Black";
        case Red:   return "Red";
        case Blue:  return "Blue";
        default:    return "?";
    }
}

void inspect_B(struct buffer *B);
void inspect_substream(struct stream *S, struct buffer *B,
                       size_t (*length)(struct stream *, struct buffer *),
                       uint64_t (*element)(struct stream *, struct buffer *,
                                           size_t));
void inspect_S_I(struct stream *S, struct buffer *B);
void inspect_S_IB(struct stream *S, struct buffer *B);
void inspect_S_B(struct stream *S, struct buffer *B);
void inspect_S_BO(struct stream *S, struct buffer *B);
void inspect_S_O(struct stream *S, struct buffer *B);
void inspect(struct stream *red, struct stream *blue, struct buffer *B);

void check_invariants(struct thread_data *td, const char *func, int line);

void thread_positions(struct stream *red, struct stream *blue);

#endif /* __DEBUGGING_H */
