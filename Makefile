CC=gcc
CFLAGS=-g -Wall
INCLUDE=-I./include/
#LIBS=-lGL -lGLU -lglut
LIBS=-framework OpenGL -framework GLUT

obj/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

smartgoku: obj/smartgoku.o obj/agent.o obj/pathsearch.o obj/utils.o obj/search.o obj/pqueue.o obj/stack.o obj/linkedlist.o
	$(CC) -o build/$@ $^ $(CFLAGS) $(LIBS)

run:
	build/smartgoku

clean:
	rm obj/*.o
