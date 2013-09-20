###########################################################
# Project 1 Makefile

CC = g++
CFLAGS = -Wall
OPFLOAGS = -ffast-math -O3
TARGET = MatrixRatio
SOURCE = MissRatio.cpp

###########################################################

matrix: MissRatio.cpp
	${CC} ${CFLAGS} -o ${TARGET} ${SOURCE} 

clean:
	rm -f *.out MatrixRatio
	
g++ MissRatio.cpp -Wall -I$TACC_PAPI_INC  -Wl,-rpath,$TACC_PAPI_LIB -L$TACC_PAPI_LIB -lpapi