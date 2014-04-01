CCW=gcc -c -Wall
PPW=g++ -c -Wall
FBX=/Applications/Autodesk/FBX\ SDK/2014.2.1
INCLUDES=-I $(FBX)/include -I /usr/local/include
LIBS=/usr/local/lib/libglfw3.a $(FBX)/lib/gcc4/ub/release/libfbxsdk.a -framework Cocoa -framework IOKit -framework CoreVideo myprogrammer.o vertex.o myshapes.o myshader.o mymesh.o mynode.o lodepng.o myfbx.o -framework opengl

all : glsl
glsl : main.cpp vertex.o myshapes.o myshader.o mymesh.o lodepng.o myprogrammer.o myfbx.o mynode.o
	g++ -o glsl main.cpp $(INCLUDES) $(LIBS)

vertex.o : vertex.c
	$(CCW) vertex.c
myshapes.o : myshapes.c
	$(CCW) myshapes.c
myshader.o : myshader.c
	$(CCW) myshader.c
mymesh.o : mymesh.cpp
	$(PPW) mymesh.cpp
lodepng.o : lodepng.c
	$(CCW) lodepng.c
myprogrammer.o : myprogrammer.cpp
	$(PPW) myprogrammer.cpp
myfbx.o : myfbx.cpp
	$(PPW) myfbx.cpp -I $(FBX)/include $(FBX)/lib/gcc4/ub/release/libfbxsdk.a
mynode.o : mynode.cpp
	$(PPW) mynode.cpp
clean:
	rm -rf *.o glsl a.out

