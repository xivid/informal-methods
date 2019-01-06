#ifndef __INTERPRETATION_H
#define __INTERPRETATION_H

#include "structures.h"

/* These functions define the interpretation of a system state (stream plus
 * shared circular buffer) as concatenated substreams.
 *
 * For a coloured stream S, length_S_X(S,B) must return its number of elements
 * in the substream S_X of S.  Likewise element_S_X(S,B,i) returns the ith
 * element of substream S_X. */
size_t length_S_I(struct stream *S, struct buffer *B);
size_t length_S_IB(struct stream *S, struct buffer *B);
size_t length_S_B(struct stream *S, struct buffer *B);
size_t length_S_BO(struct stream *S, struct buffer *B);
size_t length_S_O(struct stream *S, struct buffer *B);
uint64_t element_S_I(struct stream *S, struct buffer *B, size_t i);
uint64_t element_S_IB(struct stream *S, struct buffer *B, size_t i);
uint64_t element_S_B(struct stream *S, struct buffer *B, size_t i);
uint64_t element_S_BO(struct stream *S, struct buffer *B, size_t i);
uint64_t element_S_O(struct stream *S, struct buffer *B, size_t i);

#endif /* __INTERPRETATION_H */
