CFLAGS=-Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS=

.PHONY: all
all: proteus pasm depasm

proteus: ./src/proteus.c
	$(CC) $(CFLAGS) -o proteus ./src/proteus.c $(LIBS)

pasm: ./src/pasm.c
	$(CC) $(CFLAGS) -o pasm ./src/pasm.c $(LIBS)

depasm: ./src/depasm.c
	$(CC) $(CFLAGS) -o depasm ./src/depasm.c $(LIBS)

.PHONY: examples
examples: ./examples/fib.o ./examples/123.o

./examples/fib.o: ./examples/fib.s
	./pasm ./examples/fib.s ./examples/fib.o

./examples/123.o: ./examples/123.s
	./pasm ./examples/123.s ./examples/123.o

 