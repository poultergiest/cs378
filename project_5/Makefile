###########################################################
# Project 2 Makefile
CC = g++
CFLAGS = -O3
TARGET = final.out
SOURCE = main2.cpp utils.cpp CController.cpp C2DMatrix.cpp CMineSweeper.cpp CGenAlg.cpp CNeuralNet.cpp
CSOURCE = CController.cpp
PSOURCE = CParams.cpp
TSOURCE = CTimer.cpp
NETSOURCE = CNeuralNet.cpp
OSOURCE = SVector2D.h

###########################################################

sweepers: ${SOURCE}
	clear
	${CC} ${CFLAGS} -o ${TARGET} CParams.o ${SOURCE} -lrt -lSDL

controller: ${CSOURCE}
	clear
	${CC} ${CFLAGS} -c ${CSOURCE}

cparams: ${PSOURCE}
	clear
	${CC} ${CFLAGS} -c ${PSOURCE}

ctimer: ${TSOURCE}
	clear
	${CC} ${CFLAGS} -c ${TSOURCE}

cnet: ${NETSOURCE}
	clear
	${CC} ${CFLAGS} -c ${NETSOURCE} -msse4

clean:
	rm -f *.out {TARGET}
