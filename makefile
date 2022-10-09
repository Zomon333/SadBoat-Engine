CC = g++
FLAGS = -std=c++20 -ffreestanding -fpermissive -pthread

DEFINES = 

SOURCE_DIR = ./src
INCLUDE_DIRS = "-I./include/" "-I/usr/include/"
OUT_DIR = ./output
LIBRARY_DIR = "usr/include/GL"

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

LIBS = 

NAME = "SadBoatEngine-V"
VERSION = "0.02.7V"


prod:
	g++ $(FLAGS) $(DEFINES) $(PRODUCTION_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)""

test:
	g++ $(FLAGS) $(DEFINES) $(TEST_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"-T"

clean:
	rm $(OUT_DIR)/*


