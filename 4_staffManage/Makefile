CFDIR = src

Object = staff

.PHONY: all

all: ${Object}

staff: ${CFDIR}/staff.c
	gcc ${CFDIR}/staff.c -o staff -lncurses

	mv ${Object} ./obj

clean:
	rm ./obj/${Object}
