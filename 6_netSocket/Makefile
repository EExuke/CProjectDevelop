CFDIR = src

Object = 

.PHONY: all

all: ${Object}

 : ${CFDIR}/ .c
	gcc ${CFDIR}/ .c -o   -lncurses

	mv ${Object} ./obj

clean:
	rm ./obj/${Object}
