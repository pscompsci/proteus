CFLAGS=-Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS=

.PHONY: all
all: vmt vme

vmt: ./src/vmt.c
	 $(CC) $(CFLAGS) -o vmt ./src/vmt.c $(LIBS)

vme: ./src/vme.c
	 $(CC) $(CFLAGS) -o vme ./src/vme.c $(LIBS)

.PHONY: examples
examples: ./examples/fib.vmbt ./examples/123.vmbt

./examples/fib.vmbt: ./examples/fib.vmas
	./vmt ./examples/fib.vmas ./examples/fib.vmbt

./examples/123.vmbt: ./examples/123.vmas
	./vmt ./examples/123.vmas ./examples/123.vmbt

 