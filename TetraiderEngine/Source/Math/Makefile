PRG=mathTest.exe
FLAGS=-O2 -Wall -Wextra -ansi -pedantic -Wunknown-pragmas -Wunused -std=c++11
MSCOPEN=/EHsc /Fe
MSCARGS=opengl32.lib glew32.lib sdl2.lib sdl2main.lib /link /subsystem:console

ifeq (,$(findstring CYGWIN,$(OSTYPE)))
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

gpp:
	echo Compiling with GNU.
	g++ -c Vector2D.cpp $(FLAGS)
	g++ -c Vector3D.cpp $(FLAGS)
	g++ -c Matrix2x2.cpp $(FLAGS)
	g++ -c Matrix3x3.cpp $(FLAGS)
	g++ -c Matrix4x4.cpp $(FLAGS)
	g++ -c testMain.cpp $(FLAGS)
	g++ -o $(PRG) Matrix2x2.o Matrix3x3.o Matrix4x4.o Vector3D.o Vector2D.o testMain.o $(CYGWIN) $(FLAGS)
	
gpp2:
	echo Compiling with GNU.
	g++ -c MathDefs.h $(FLAGS)
	g++ -o $(PRG) MathDefs.o $(CYGWIN) $(FLAGS)
	
msc:
	echo Compiling with VS.
	cl $(MSCOPEN)$(PRG) main.cpp Matrix.cpp Matrix2x2.cpp Matrix3x3.cpp Matrix4x4.cpp Vector3D.cpp Vector2D.cpp LineSegment2D.cpp Math2D.cpp $(MSCARGS)

vector:
	$(PRG) -v
	
mem:
	drmemory.exe -show_reachable $(PRG)
	
clean:
	rm -f *.exe *.tds *.o *.obj *manifest*