#ifndef __INITIALISATION_H
#define __INITIALISATION_H

#include "structures.h"

void init_buffer_locals(struct buffer *B, size_t N_B);
void init_stream_locals(struct stream *S, enum colour colour, size_t N);

#endif /* __INITIALISATION_H */
