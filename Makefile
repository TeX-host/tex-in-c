#CC=/usr/bin/g++
CC=gcc
CFLAGS = -g -O -pedantic  -Wall -W
# -Wstrict-prototypes -Wmissing-prototypes
#-Wstrict-prototypes
#-Wmissing-prototypes
#-fprofile-arcs -ftest-coverage
vpath %.ppc ./ppc
STR2NUM=./pl/str2num.pl
MK_POOL=./pl/mk_pool.pl
PPC_DIR=./ppc

OBJS=tex.o funcs.o inipool.o inputln.o printout.o str.o fonts.o dviout.o
PSRCS=tex.ppc inputln.ppc printout.ppc str.ppc fonts.ppc dviout.ppc
CSRCS=$(OBJS:.o=.c)


all: ttex 

deps: ${CSRCS}
	-rm -f deps.1
	for A in ${CSRCS} ; do \
		$(CC) -MM $$A >> deps.1 ; \
	done
	cmp deps deps.1 || mv deps.1 deps

pool_str.c: ${PSRCS}
	perl $(MK_POOL) $(addprefix $(PPC_DIR)/,${PSRCS})
	cmp pool_str.c1 pool_str.c || cp pool_str.c1 pool_str.c

tex.c: tex.ppc pool_str.c
	perl $(STR2NUM) $(PPC_DIR)/tex.ppc > tex.c

dviout.c: dviout.ppc pool_str.c
	perl $(STR2NUM) $(PPC_DIR)/dviout.ppc > dviout.c

inputln.c: inputln.ppc pool_str.c global.h
	perl $(STR2NUM) $(PPC_DIR)/inputln.ppc > inputln.c

printout.c: printout.ppc pool_str.c
	perl $(STR2NUM) $(PPC_DIR)/printout.ppc > printout.c

str.c: str.ppc pool_str.c
	perl $(STR2NUM) $(PPC_DIR)/str.ppc > str.c

fonts.c: fonts.ppc pool_str.c
	perl $(STR2NUM) $(PPC_DIR)/fonts.ppc > fonts.c

ttex: ${OBJS}
	${CC} -o ttex ${OBJS} -lm


clean: 
	-rm -f ttex ${OBJS} pool_str.c1

distclean: clean
	-rm -f $(PSRCS:.ppc=.c) pool_str.c

.PHONY: clean distclean deps

include deps
