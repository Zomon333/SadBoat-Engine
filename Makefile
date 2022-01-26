CC = cl
FLAGS = /std:C++17 /WX /EHsc
OBJECTS = main.obj
HEADERS =
VERSION = 0.01.7D

SadBoatEngine: $(OBJECTS)
	$(CC) /Fe"SadBoatEngine-V$(VERSION).exe" $(OBJECTS)

main.obj: src\main.cpp $(HEADERS)
	$(CC) $(FLAGS) /I lib\ -c src\main.cpp

clean:
	del *.exe *.obj

remake:
	nmake clean
	nmake