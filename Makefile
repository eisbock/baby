all: baby_test libbaby.a

test: baby_test

clean:
	rm -f baby_test libbaby.a *.o

baby.o: baby.c
	gcc -Wall -c $< -o $@

libbaby.a: baby.o
	ar rcs $@ $^

baby_test: baby_test.c libbaby.a
	gcc -Wall -o $@ baby_test.c baby.c -lgmp -L. -lbaby

