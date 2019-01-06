#ifndef __PREDICATES_H
#define __PREDICATES_H

#include "structures.h"

/*** Invariants ***/

bool Inv_I(struct stream *S, struct buffer *B);
bool Inv_O(struct stream *S, struct buffer *B);
bool Inv_T(struct stream *red, struct stream *blue, struct buffer *B);
bool Inv_B(struct buffer *B);
bool Inv_S(struct stream *S, struct buffer *B);

/*** Other Predicates (postconditions, etc.) */

bool Postcondition(struct stream *S);

#endif /* __PREDICATES_H */
