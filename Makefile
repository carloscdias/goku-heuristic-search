CC=gcc
CFLAGS=-g -Wall
INCLUDE=-I./include/
#LIBS=-lGL -lGLU -lglut
LIBS=-framework OpenGL -framework GLUT

obj/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

smartgoku: obj/smartgoku.o obj/agent.o obj/exploresearch.o obj/pathsearch.o obj/search.o obj/utils.o obj/pqueue.o obj/stack.o obj/linkedlist.o
	$(CC) -o build/$@ $^ $(CFLAGS) $(LIBS)

run:
	build/smartgoku

clean:
	rm obj/*.o
