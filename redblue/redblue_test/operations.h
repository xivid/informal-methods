#ifndef __OPERATIONS_H
#define __OPERATIONS_H

#include "structures.h"

void push(struct stream *S, struct buffer *B, struct thread_data *td);
void pop(struct stream *S, struct buffer *B, struct thread_data *td);

#endif /* __OPERATIONS_H */
