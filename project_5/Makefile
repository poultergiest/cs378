###########################################################
# Project 2 Makefile

CC = g++
CFLAGS = -O3
TARGET = final.out
SOURCE = main2.cpp CMineSweeper.cpp SVector2D.h utils.cpp CController.cpp CGenAlg.cpp utils.cpp C2DMatrix.cpp CParams.cpp
CSOURCE = CMineSweeper.cpp

###########################################################

sweepers: ${SOURCE}
	clear
	${CC} ${CFLAGS} -o ${TARGET} ${SOURCE} -lrt -lSDL

controller: ${CSOURCE}
	clear
	${CC} ${CFLAGS} -c ${CSOURCE}

clean:
	rm -f *.out {TARGET}
