CFDIR = src

Object = consume

.PHONY: all

all: ${Object}

consume: ${CFDIR}/consume.c
	gcc ${CFDIR}/consume.c -o consume -lncurses

	mv ${Object} ./obj

clean:
	rm ./obj/${Object}
