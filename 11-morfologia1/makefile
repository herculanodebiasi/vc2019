CC = g++
CFLAGS = -g -Wall
PROG = morfologia
SRCS = $(PROG).cpp

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
