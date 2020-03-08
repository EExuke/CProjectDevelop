CFDIR = src

Object = superCalendar

.PHONY: all

all: ${Object}

superCalendar: ${CFDIR}/superCalendar.c
	gcc ${CFDIR}/superCalendar.c -o superCalendar -lncurses

	mv ${Object} ./obj

clean:
	rm ./obj/${Object}
