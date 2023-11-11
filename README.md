# baby
A hashtable of mpz\_t values (libgmp), intended to be useful in a baby-step giant-step algorithm to compute a discrete log.

This library will creates a hashtable of small numbers in the ring (a\*\*i % n).
You can lookup the modular values to find the offset that created it. It is
intended for use in a baby-step giant-step algorithm, although I would be
interested in hearing about unintended usefulness.

The test (`make test`) will search for an r where 2\*\*(n-2r+1) == 1 mod n. This
is equivalent to factoring, since that r is part of a hyperbolic pair with an
h = sqrt(r\*\*2 -n), such that p=r+h and q=r-h. The test is a toy using a small
hashtable, but the concept can be used at larger scale, given enough memory.

