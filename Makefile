#CC=/usr/bin/g++
CC=gcc
CFLAGS = -g -pedantic -Wall -Wno-unused-result
# -Wstrict-prototypes -Wmissing-prototypes
#-Wstrict-prototypes
#-Wmissing-prototypes
#-fprofile-arcs -ftest-coverage

OBJS=tex.o funcs.o inipool.o inputln.o printout.o str.o fonts.o dviout.o
CSRCS=$(OBJS:.o=.c)


all: ttex
ttex: ${OBJS}
	${CC} -o ttex ${OBJS} -lm

.PHONY: clean deps
clean: 
	-rm -f ttex ${OBJS}

deps: ${CSRCS}
	-rm -f deps.1
	for A in ${CSRCS} ; do \
		$(CC) -MM $$A >> deps.1 ; \
	done
	cmp deps deps.1 || mv deps.1 deps

include deps
