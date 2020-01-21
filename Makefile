##To make file type make
CC = g++
CFLAGS = -g -Wall -Iinclude

HEADER = include/imageProcessing.h
OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

SRCS= imageProcessing.cpp main.cpp

OBJS = $(SRCS:.c++=.o)

MAIN = main

.PHONY: depend clean

all:    $(MAIN)
		@echo  Message: executable file ...main... has been created

$(MAIN): $(OBJS) 
		$(CC) $(CFLAGS) $(HEADER) -o $(MAIN) $(OBJS) $(LIBS)

.c++.o:
		$(CC) $(CFLAGS) $(HEADER) -cpp $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
		makedepend $(HEADER) $^
