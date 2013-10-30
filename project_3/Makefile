###########################################################
# Project 2 Makefile

CC = g++
CFLAGS = -O3
TARGET = proj3-1.out
TARGET2 = proj3-2.out
SOURCE = graphvis.c++ 
SOURCE2 = fastgraphvis.c++

###########################################################

graphvis: ${SOURCE}
	clear
	${CC} ${CFLAGS} -o ${TARGET} ${SOURCE} -lrt -lSDL

fastgraph: ${SOURCE2}
	clear
	${CC} ${CFLAGS} -o ${TARGET2} ${SOURCE2} -lrt -lSDL

clean:
	rm -f *.out {TARGET} {TARGET2}
