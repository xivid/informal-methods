COMMON_SOURCES= \
    redblue.c

USER_SOURCES= \
    ${COMMON_SOURCES} \
    debugging.c \
    initialisation.c \
    interpretation.c \
    operations.c \
    predicates.c

REF_SOURCES= \
    ${COMMON_SOURCES} \
    debugging_ref.c \
    initialisation_ref.c \
    interpretation_ref.c \
    operations_ref.c \
    predicates_ref.c

COMMON_HEADERS= \
    atomics.h \
    debugging.h \
    initialisation.h \
    interpretation.h \
    operations.h \
    predicates.h

USER_HEADERS= \
    ${COMMON_HEADERS} \
    structures.h

REF_HEADERS= \
    ${COMMON_HEADERS} \
    structures_ref.h

.PHONY: clean package

redblue: ${USER_SOURCES} ${USER_HEADERS}
	gcc -Wall -Werror -std=c11 -g -o $@ ${USER_SOURCES} -lpthread

redblue_ref: ${REF_SOURCES} ${REF_HEADERS}
	gcc -DBUILD_REF -Wall -Werror -std=c11 -g -o $@ ${REF_SOURCES} -lpthread

package: redblue_test.tar.gz

redblue_test.tar.gz: Makefile ${USER_SOURCES} ${USER_HEADERS}
	mkdir redblue_test
	cp Makefile ${USER_SOURCES} ${USER_HEADERS} redblue_test
	tar cfz $@ redblue_test
	rm -rf redblue_test

clean:
	rm -rf redblue redblue_ref redblue_test.tar.gz redblue_test
