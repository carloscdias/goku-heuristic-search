CC=gcc
CFLAGS=-g -Wall
INCLUDE=-I./include/

ifeq ($(shell uname), Linux)
	LIBS=-lGL -lGLU -lglut -lpthread
else
	LIBS=-framework OpenGL -framework GLUT
endif

obj/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

all: smartgoku geneticpath

smartgoku: obj/smartgoku.o obj/ghsgraphics.o obj/agent.o obj/rateexploresearch.o obj/tspsearch.o obj/pathsearch.o obj/astar.o obj/ghsdata.o obj/pqueue.o obj/stack.o obj/linkedlist.o
	$(CC) -o build/$@ $^ $(CFLAGS) $(LIBS)

geneticpath: obj/geneticpath.o obj/agent.o obj/rateexploresearch.o obj/tspsearch.o obj/pathsearch.o obj/astar.o obj/ghsdata.o obj/pqueue.o obj/stack.o obj/linkedlist.o
	$(CC) -o build/$@ $^ $(CFLAGS) -lpthread 

run:
	build/smartgoku

run_geneticpath:
	build/geneticpath

clean:
	rm obj/*.o
