CC = g++
CFLAGS = -g -Wall
PROG = olaLena
SRCS = $(PROG).cpp

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
