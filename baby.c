// See baby.h for code docs
//
// Copyright 2023 Jesse Dutton

#include "baby.h"


struct baby_item {
  struct baby_item *next;
  unsigned int offset;
};


struct baby {
  mpz_t n;
  mpz_t a;
  mpz_t scratch;
  struct baby_item **idx;
  struct baby_item *items;
  unsigned long idx_size;
  unsigned long step;
};


void baby_free(struct baby *b) {
  if (b) {
    free(b->idx);
    free(b->items);
    mpz_clears(b->n, b->a, b->scratch, NULL);
    free(b);
  }
}


struct baby * baby_make(const mpz_t n,
                        unsigned long a,
                        unsigned long step,
                        unsigned long num_items,
                        unsigned long idx_size)
{
  unsigned long i,j;
  struct baby *b;

  // alloc
  b = calloc(1, sizeof(struct baby));
  b->idx = calloc(idx_size, sizeof(struct baby_item *));
  b->items = calloc(num_items, sizeof(struct baby_item));
  b->idx_size = idx_size;
  b->step = step;

  // init
  mpz_init_set(b->n, n);
  mpz_init_set_ui(b->a, a);
  mpz_init(b->scratch);

  // populate backwards, so that small numbers are found first
  for (i = num_items -1; i >0; i--) {
    mpz_powm_ui(b->scratch, b->a, step*i, b->n);
    j = mpz_get_ui(b->scratch);
    j = j % b->idx_size;

    b->items[i].next = b->idx[j];
    b->items[i].offset = i;
    b->idx[j] = b->items + i;
  }

  return b;
}


int baby_lookup(unsigned long *ret, struct baby *b, const mpz_t x) {
  struct baby_item *cur;
  unsigned int i, j;

  // iterate through the list at idx[j]
  j = mpz_get_ui(x);
  j = j % b->idx_size;
  cur = b->idx[j];

  while ((cur)) {
    i = cur->offset;

    // expand into the full value for compare
    mpz_set_ui(b->scratch, b->step*i);
    mpz_powm(b->scratch, b->a, b->scratch, b->n);

    if (mpz_cmp(b->scratch, x) == 0) {
      *ret = i;
      return 1;
    }
    cur = cur->next;
  }

  return 0;
}

