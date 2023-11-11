// Attempt to find r such that 2**(n-2r+1) == 1 mod n, using the baby-step
// giant-step algorithm and the baby data structure implemented in this repo.
//
// When found, r = (p+q)/2  and h**2 = r**2 -n such that p=r+h, q=r-h
// Finding r for a given n is equivalent to factoring, since the rest is
// just algebra. We use the algorithm for the discrete log part, and test the
// baby data structure as we iterate.
//
// gcc -Wall baby_test.c baby.c -lgmp

#include <stdio.h>
#include "baby.h"

#define IDX_SZ 16384
#define GIANT_SZ 65536

int main(int argc, char **argv) {
  struct baby *b;
  mpz_t n, r, giant, m, two;
  unsigned long off;

  if (argc > 2) {
    printf("Usage:  %s [num]\n", argv[0]);
    return 1;
  } else if (argc == 2) {
    mpz_init_set_str(n, argv[1], 0);
  } else {
    mpz_init_set_str(n, "491020194313983920358193", 0);
    printf("r=712465681567 (expected)\n");
  }
  mpz_inits(r, giant, m, two, NULL);
  mpz_set_ui(two, 2);

  // starting r = ceil(sqrt(n))
  mpz_sqrt(r, n);
  mpz_add_ui(r, r, 1);

  // m = 2**(n-2r+1) % n
  mpz_mul(m, two, r);
  mpz_sub(m, n, m);
  mpz_add_ui(m, m, 1);
  mpz_powm(m, two, m, n);

  // giant step
  mpz_set_si(giant, -2*GIANT_SZ);
  mpz_powm(giant, two, giant, n);

  // make the baby
  b = baby_make(n, 2, 2, GIANT_SZ, IDX_SZ);

  // baby-step giant-step algo
  while (baby_lookup(&off, b, m) == 0) {
    mpz_add_ui(r, r, GIANT_SZ);
    mpz_mul(m, m, giant);
    mpz_mod(m, m, n);
  }
  mpz_add_ui(r, r, off);

  // print, cleanup
  gmp_printf("r=%Zd\n", r);
  baby_free(b);
  mpz_clears(n, r, giant, m, two, NULL);

  return 0;
}
