###########################################################
# Project 2 Makefile

CC = g++
CFLAGS = -Wall -O3
SSE = -msse3
TARGET = proj.out
SOURCE = VectorizedMMM.cpp

###########################################################

sha: SHA.cpp
	clear
	${CC} ${CFLAGS} SHA.cpp -lcrypto -lrt

project: VectorizedMMM.cpp
	clear
	${CC} ${CFLAGS} -o ${TARGET} -ffast-math -fno-tree-vectorize ${SOURCE}

clean:
	rm -f *.out {TARGET}