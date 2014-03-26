CCW=gcc -c -Wall
all : glsl
glsl : vertex.o myshapes.o myshader.o mymesh.o
#	g++ -o glsl main.cpp -I /usr/include/GL /usr/lib/libGLEW.a vertex.o myshapes.o myshader.o -framework opengl -framework glut
	g++ -o glsl main.cpp vertex.o myshapes.o myshader.o mymesh.o -framework opengl -framework glut
vertex.o : vertex.c
	$(CCW) vertex.c
myshapes.o : myshapes.c
	$(CCW) myshapes.c
myshader.o : myshader.c
	$(CCW) myshader.c
mymesh.h : mymesh.cpp
	g++ -c -Wall mymesh.c
clean:
	rm -rf *.o glsl a.out

