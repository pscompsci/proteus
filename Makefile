CFLAGS=-Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS=

.PHONY: all
all: vmt vme

vmt: vmt.c
	 $(CC) $(CFLAGS) -o vmt vmt.c $(LIBS)

vme: vme.c
	 $(CC) $(CFLAGS) -o vme vme.c $(LIBS)

vm: main.c
	$(CC) $(CFLAGS) -o vm main.c $(LIBS)

.PHONY: examples
examples: ./examples/fib.vmbt ./examples/123.vmbt

./examples/fib.vmbt: ./examples/fib.vmas
	./vmt ./examples/fib.vmas ./examples/fib.vmbt

./examples/123.vmbt: ./examples/123.vmas
	./vmt ./examples/123.vmas ./examples/123.vmbt

 