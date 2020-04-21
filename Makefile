CFLAGS=-std=c11 -g -static

ftftcc: ftftcc.c

test:ftftcc
	./test.sh

clean:
	rm -f ftftcc *.o *~ tmp*

.PHONY: test clean