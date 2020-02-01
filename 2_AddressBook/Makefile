CFDIR = src

Object = addressBook

.PHONY: all

all: ${Object}

addressBook: ${CFDIR}/addressBook.c
	gcc ${CFDIR}/addressBook.c -o addressBook -ltinfo -lncurses

	mv ${Object} ./obj

clean:
	rm ./obj/${Object}
