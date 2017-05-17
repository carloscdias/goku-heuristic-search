CC=gcc
CFLAGS=-Wall
INCLUDE=-I./include/
LIBS=-lGL -lGLU -lglut
#LIBS=-framework OpenGL -framework GLUT

obj/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

smartgoku: obj/smartgoku.o obj/agent.o
	$(CC) -o build/$@ $^ $(CFLAGS) $(LIBS)

clean:
	rm obj/*.o
