CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
ftftcc: $(OBJS)
	$(CC) -o ftftcc $(OBJS) $(LDFLAGS)

$(OBJS): ftftcc.h

test: ftftcc
	./test.sh

clean:
	rm -f ftftcc *.o *~ tmp*

.PHONY: test clean