CC = cl
FLAGS = /std:c++20 /WX /EHsc /Gd /JMC /permissive- /GS /Zi /Gm- /Od /sdl /MD
OBJECTS = main.obj
HEADERS = includes/GLFW/glfw3.h includes/glad/glad.h
LIBS = lib/glfw3.lib opengl32.lib glu32.lib lib/assimp.lib lib/freetype.lib lib/irrKlang.lib lib/STB_IMAGE.lib lib/GLAD.lib lib/glew32s.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib lib/SOIL.lib odbc32.lib odbccp32.lib
VERSION = 0.01.9D

SadBoatEngine: $(OBJECTS)
	$(CC) /Fe"SadBoatEngine-V$(VERSION).exe" $(OBJECTS) $(LIBS)

main.obj: src\main.cpp $(HEADERS)
	$(CC) $(FLAGS) src\main.cpp $(LIBS) /I includes\ /I lib\

clean:
	del *.exe *.obj *.ilk *.pdb

remake:
	nmake clean
	nmake