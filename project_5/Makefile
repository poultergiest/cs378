###########################################################
# Project 2 Makefile

CC = g++
CFLAGS = -O3
TARGET = final.out
SOURCE = main2.cpp utils.cpp

###########################################################

sweepers: ${SOURCE}
	clear
	${CC} ${CFLAGS} -o ${TARGET} ${SOURCE} -lrt -lSDL

clean:
	rm -f *.out {TARGET}
