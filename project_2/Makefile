###########################################################
# Project 2 Makefile

CC = g++
CFLAGS = -Wall -O3
SSE = -msse3
TARGET = proj.out
SOURCE = VectorizedMMM.cpp

###########################################################

project: VectorizedMMM.cpp
	clear
	${CC} ${CFLAGS} ${SSE} -o ${TARGET} ${SOURCE}

clean:
	rm -f *.out {TARGET}