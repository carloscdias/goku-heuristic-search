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

smartgoku: obj/smartgoku.o obj/ghsgraphics.o obj/agent.o obj/rateexploresearch.o obj/pathsearch.o obj/astar.o obj/ghsdata.o obj/pqueue.o obj/stack.o obj/linkedlist.o
	$(CC) -o build/$@ $^ $(CFLAGS) $(LIBS)

run:
	build/smartgoku

clean:
	rm obj/*.o
