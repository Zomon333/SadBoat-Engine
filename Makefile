CC = cl
FLAGS = /std:c++20 /WX /EHsc /Gd /JMC /permissive- /GS /Zi /Gm- /Od /sdl /MD
OBJECTS = main.obj
HEADERS = includes/GLFW/glfw3.h includes/glad/glad.h
LIBS = lib/GLAD.lib lib/glfw3.lib opengl32.lib glu32.lib lib/assimp.lib lib/freetype.lib lib/irrKlang.lib lib/STB_IMAGE.lib lib/glew32s.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib lib/SOIL.lib odbc32.lib odbccp32.lib
VERSION = 0.02.7D

SadBoatEngine: $(OBJECTS)
	$(CC) /Fe"SadBoatEngine-V$(VERSION).exe" $(LIBS) $(OBJECTS) 

main.obj: src\main.cpp includes\glad.c $(HEADERS)
	$(CC) $(FLAGS) src\main.cpp includes\glad.c $(LIBS) /I includes\ /I lib\;
	del main.exe

clean:
	del *.exe *.obj *.ilk *.pdb

remake:
	nmake clean
	nmake

	
compileTest:
	nmake remake
	nmake clean