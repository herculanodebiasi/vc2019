CC = g++
CFLAGS = -g -Wall
PROG = opengl
SRCS = $(PROG).cpp

OPENCV = `pkg-config opencv --cflags --libs` -lGL -lglut -lm
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
