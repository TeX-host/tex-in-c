CFLAGS = -g -O0 -I.
CFLAGS += -pedantic -Wall -Wno-unused-result
CFLAGS += --coverage -ftest-coverage -fprofile-arcs

OBJS=tex.o funcs.o inipool.o inputln.o printout.o str.o fonts.o dviout.o
CSRCS=$(OBJS:.o=.c)


all: ttex
ttex: ${OBJS}
	${CC} -o ttex ${OBJS} -lm ${CFLAGS}


.PHONY: clean deps test
clean:
	-rm -f ttex ${OBJS}
	-rm -f test/*.dvi test/*.log
	-rm -f *.gcno *.gcda *.gcov

deps: ${CSRCS}
	-rm -f deps.1
	for A in ${CSRCS} ; do \
		$(CC) -MM $$A >> deps.1 ; \
	done
	cmp deps deps.1 || mv deps.1 deps

test: ttex
	./ttex test/helloworld
	gcov tex.c

include deps
