###########################################################
# Project 1 Makefile

CC = g++
CFLAGS = -Wall
OPFLAGS = -I${TACC_PAPI_INC} -Wl,-rpath,${TACC_PAPI_LIB} -L${TACC_PAPI_LIB} -lpapi
OPFLAGS2 = -ffast-math -O3 -fno-tree-vectorize
TARGET = MissRatio.out
TARGET2 = FastMatrixMultiply.out
SOURCE = MissRatio.cpp
SOURCE2 = FastMatrixMultiply.cpp

###########################################################

matrix: MissRatio.cpp
	${CC} ${CFLAGS} -o ${TARGET} ${SOURCE}

papi: MissRatio.cpp
	${CC} ${CFLAGS} -o ${TARGET} ${OPFLAGS} ${SOURCE}

f: ${SOURCE2}
	${CC} ${CFLAGS} -o ${TARGET2} ${OPFLAGS2} ${SOURCE2}

clean:
	rm -f *.out {TARGET}