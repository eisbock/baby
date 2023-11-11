// baby.h
//
// This creates a hashtable of small numbers in the ring (a**i % n)
// You lookup the modular values to find the offset that created it.
// Intended for use in a baby-step giant-step algorithm, although I would be
// interested in hearing about unintended usefulness.

#ifndef BABY_H
#define BABY_H

#include <stdlib.h>
#include <gmp.h>

struct baby;

// cleanup memory used by the baby hashtable
void baby_free(struct baby *b);

// construct a hashtable for small numbers in the ring (a**i % n)
//
// The table will have num_items starting at 0 and using step to make a list.
// The value of step is almost always either 1 or 2. For example, the totient
// function means that semiprime n with factors p,q will have a congruence
//     a**(n-p-q+1) % n == 1
// Meaning that possible values of (p+q) can be compared to (n+1). However,
// p+q is always even, so you may not wish to store the odd values in the
// baby. In that case, step=2 will accomplish this.
//
// The index will have idx_size entries, slightly smaller than num_items.
// Allocated memory: 56 + 16*num_items + 8*idx_size + 2*(bytes in n)
// So, with num_items = 2**32 and idx_size = 2**30, this will alloc ~72GB
//
// This function allocates the memory and fills in the values. Only the low
// 64-bit word is hashed (mod idx_size). This means that an mpz_t is not
// stored, for memory efficiency reasons. As a result, the mpz_t is
// reconstructed from the offset i during lookup and compared. As a result,
// the size difference between num_items and idx_size will determine how many
// reconstructions might be needed during lookup. In our example, we would
// expect an average of 4 (because of a difference of 2 bits).
struct baby * baby_make(const mpz_t n,
                        unsigned long a,
                        unsigned long step,
                        unsigned long num_items,
                        unsigned long idx_size);

// lookup a value in the hashtable and recover the unsigned long offset that
// created it. If step was not 1, the offset returned is the number of steps,
// or i, not step*i. Returns 1 and sets ret if found. Otherwise, returns 0.
// 
// Performance depends on the ratio of num_items and idx_size when created.
// See the comments for baby_make for more information.
int baby_lookup(unsigned long *ret, struct baby *b, const mpz_t x);

#endif
