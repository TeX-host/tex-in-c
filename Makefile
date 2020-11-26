CFLAGS = -g -O0 -I. -std=c11
CFLAGS += -pedantic -Wall -Wno-unused-result
LDFLAGS = -lm

SRCS  = tex
SRCS += charset
SRCS += io io_unix io_fname
SRCS += texmath str printout print hash
SRCS += box box_display box_destroy box_copy
SRCS += eqtb eqtb_save
SRCS += mmode math_formula
SRCS += align linebreak
SRCS += hyphen_pre hyphen_post hyphen hyphen_init
SRCS += pagebreak page_builder
SRCS += main_ctrl
SRCS += dump debug extension
SRCS += lexer fonts dviout expand scan error mem pack
CSRCS = $(addsuffix .c,${SRCS})
OBJS  = $(addsuffix .o,${SRCS})

# 输出二进制的文件名
EXEC_NAME = ttex


## 编译
all: ${EXEC_NAME}

${EXEC_NAME}: deps ${OBJS}
	${CC} ${CFLAGS} -o ${EXEC_NAME} ${OBJS} ${LDFLAGS}


## 测试与运行
debug: clean_test ${EXEC_NAME}
	cp ${EXEC_NAME} test/trip
	cp ${EXEC_NAME} _test

# clean && run && load plainTeX
r: debug
	cd _test && ./${EXEC_NAME} plain

## Testset ======================================
basic-test: RUN_DIR := test/basic-test
basic-test: clean_test ${EXEC_NAME}
	./${EXEC_NAME} ${RUN_DIR}/end
	./${EXEC_NAME} ${RUN_DIR}/helloworld
	@echo "---------- basic-test end ----------"

other-test: RUN_DIR := test
other-test: clean_test ${EXEC_NAME}
	./${EXEC_NAME} ${RUN_DIR}/E560
	./${EXEC_NAME} ${RUN_DIR}/bug-show
	@echo "---------- other-test end ----------"

trip: debug
	cd test/trip && ./${EXEC_NAME} --indep "\input trip"
	cd test/trip && mv trip.log tripin.log
	cd test/trip && ./${EXEC_NAME} --indep "&trip" trip

test t: debug basic-test other-test

# Use Target-specific Variable: set CFLAGS
codecov: CFLAGS := ${CFLAGS} --coverage -ftest-coverage -fprofile-arcs
codecov: clean ${EXEC_NAME} test
	gcov tex.c


## 清理
.PHONY: clean clean_test debug r test t codecov
clean:
	-rm -f ${EXEC_NAME} ${OBJS}
	-rm -f test/*.dvi test/*.log
	-rm -f *.gcno *.gcda *.gcov

clean_test: clean
	-rm -f test/${EXEC_NAME} test/*.dvi test/*.log
	-rm -f test/trip/${EXEC_NAME} test/trip/*.dvi test/trip/*.log
	-rm -f _test/${EXEC_NAME} _test/*.dvi _test/*.log


## 自动产生依赖
deps: ${CSRCS}
	-rm -f deps.1
	for A in ${CSRCS} ; do \
		${CC} -MM $$A >> deps.1 ; \
	done
	cmp deps deps.1 || mv deps.1 deps

include deps
