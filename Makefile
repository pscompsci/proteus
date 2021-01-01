CFLAGS=-Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS=

.PHONY: all
all: prot prott

prot: ./src/proteus.c
	 $(CC) $(CFLAGS) -o prot ./src/proteus.c $(LIBS)

prott: ./src/proteust.c
	 $(CC) $(CFLAGS) -o prott ./src/proteust.c $(LIBS)

.PHONY: examples
examples: ./examples/fib.pb ./examples/123.pb

./examples/fib.pb: ./examples/fib.pasm
	./prott ./examples/fib.pasm ./examples/fib.pb

./examples/123.pb: ./examples/123.pasm
	./prott ./examples/123.pasm ./examples/123.pb

 