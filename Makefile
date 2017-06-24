# CONFIG

CC := g++
SRCDIR ?= src
BUILDDIR ?= build
DEPDIR ?= dep
BINDIR ?= bin
TESTSDIR ?= tests
SRCEXT ?= cpp
CFLAGS += -std=c++14 -O2 -ffast-math -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections -larmadillo
INCLUDE += -I include

# /CONFIG

TARGET_SOURCES := ${wildcard ${SRCDIR}/target/*.${SRCEXT}}
TARGET_ELFS := ${patsubst ${SRCDIR}/target/%.${SRCEXT},bin/%,${TARGET_SOURCES}}

TEST_SOURCES := ${wildcard ${SRCDIR}/tests/*.${SRCEXT}}
TEST_ELFS := ${patsubst ${SRCDIR}/tests/%.${SRCEXT},tests/%,${TEST_SOURCES}}

LIB_SOURCES := ${wildcard ${SRCDIR}/*.${SRCEXT}}
SOURCES := ${TARGET_SOURCES} ${TEST_SOURCES} ${LIB_SOURCES}
OBJECTS := ${patsubst ${SRCDIR}/%.${SRCEXT},${BUILDDIR}/%.o,${SOURCES}}
LIB_OBJECTS := ${patsubst ${SRCDIR}/%.${SRCEXT},${BUILDDIR}/%.o,${LIB_SOURCES}}
DEPENDS := ${patsubst ${SRCDIR}/%.${SRCEXT},${DEPDIR}/%.d,${SOURCES}}

target: ${TARGET_ELFS}
	
tests: ${TEST_ELFS}
	
all: target tests
	
include ${DEPENDS}

${DEPDIR}/%.d: ${SRCDIR}/%.${SRCEXT}
	@mkdir -p ${shell dirname $@} ; \
	echo -n "`dirname '${patsubst ${SRCDIR}/%.${SRCEXT},${BUILDDIR}/%.o,$<}'`/" > $@; \
	$(CC) -MM ${INCLUDE} $< >> $@
${BINDIR}/% : ${BUILDDIR}/target/%.o ${LIB_OBJECTS}
	$(CC) ${LDFLAGS} $^ -o $@ ${CFLAGS}
${TESTSDIR}/% : ${BUILDDIR}/tests/%.o ${LIB_OBJECTS}
	$(CC) ${LDFLAGS} $^ -o $@ ${CFLAGS}
${BUILDDIR}/%.o: ${SRCDIR}/%.${SRCEXT}
	@mkdir -p `dirname $@` ;\
	echo '$(CC) -c ${INCLUDE} ${CFLAGS} $< -o $@' ;\
	      $(CC) -c ${INCLUDE} ${CFLAGS} $< -o $@
dirs:
	mkdir -p ${SRCDIR} ${BUILDDIR} ${DEPDIR} ${BINDIR} ${TESTSDIR} ${SRCDIR}/target ${SRCDIR}/tests

clean:
	rm -rf ${BUILDDIR}/*	\
	       ${BINDIR}/*	\
	       ${TESTSDIR}/*    \
	       $(DEPDIR)/*;

.PHONY: target tests all dirs clean
	
.SECONDARY: ${OBJECTS} ${TARGET_ELFS} ${TEST_ELFS}
	
