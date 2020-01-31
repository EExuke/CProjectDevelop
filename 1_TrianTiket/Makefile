CFDIR = src

Object = train

.PHONY: all

all: ${Object}

train: ${CFDIR}/train.c
	gcc ${CFDIR}/train.c -o train -ltinfo -lncurses

	mv ${Object} ./obj

clean:
	rm ./obj/${Object}
