CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

ParseGPX: gpx.o
	${CC} ${CFLAGS} -o $@ $^