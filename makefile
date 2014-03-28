CCW=gcc -c -Wall
all : glsl
glsl : vertex.o myshapes.o myshader.o mymesh.o lodepng.o myprogrammer.o
	g++ -o glsl main.cpp -I /usr/local/include /usr/local/lib/libglfw3.a -framework Cocoa -framework IOKit -framework CoreVideo myprogrammer.o vertex.o myshapes.o myshader.o mymesh.o lodepng.o -framework opengl

vertex.o : vertex.c
	$(CCW) vertex.c
myshapes.o : myshapes.c
	$(CCW) myshapes.c
myshader.o : myshader.c
	$(CCW) myshader.c
mymesh.o : mymesh.cpp
	g++ -c -Wall mymesh.cpp
lodepng.o : lodepng.c
	$(CCW) lodepng.c
myprogrammer.o : myprogrammer.cpp
	g++ -c -Wall myprogrammer.cpp
clean:
	rm -rf *.o glsl a.out

