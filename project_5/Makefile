###########################################################
# Project 2 Makefile
CC = g++
CFLAGS = -O3
TARGET = final.out
SOURCE = main2.cpp utils.cpp CTimer.cpp CParams.cpp CController.cpp C2DMatrix.cpp CMineSweeper.cpp CGenAlg.cpp CNeuralNet.cpp
CSOURCE = CController.cpp
PSOURCE = CParams.cpp
NETSOURCE = CNeuralNet.cpp
OSOURCE = SVector2D.h

###########################################################

sweepers: ${SOURCE}
	clear
	${CC} -o ${TARGET} ${SOURCE} -lrt -lSDL -msse4

controller: ${CSOURCE}
	clear
	${CC} ${CFLAGS} -c ${CSOURCE}

cnet: ${NETSOURCE}
	clear
	${CC} ${CFLAGS} -c ${NETSOURCE} -msse4

clean:
	rm -f *.out {TARGET}
