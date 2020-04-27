CFLAGS = -g -O0 -I. -std=c11
CFLAGS += -pedantic -Wall -Wno-unused-result
CFLAGS += --coverage -ftest-coverage -fprofile-arcs

OBJS=tex.o funcs.o inipool.o inputln.o printout.o str.o fonts.o dviout.o pure_func.o texmath.o print.o lexer.o
CSRCS=$(OBJS:.o=.c)


all: ttex
ttex: ${OBJS}
	${CC} -o ttex ${OBJS} -lm ${CFLAGS}

debug: clean_test ttex
	cp ttex _test

# clean && run && load plainTeX
r: debug
	cd _test && ./ttex plain

.PHONY: clean deps test
clean:
	-rm -f ttex ${OBJS}
	-rm -f test/*.dvi test/*.log
	-rm -f *.gcno *.gcda *.gcov

clean_test: clean
	-rm -f _test/ttex _test/*.dvi _test/*.log

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
