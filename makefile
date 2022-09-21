CC = g++
FLAGS = -std=c++20 -fpermissive

SOURCE_DIR = ./src
INCLUDE_DIR = ./include
OUT_DIR = ./output
LIBRARY_DIR = "usr/include/GL"

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

LIBS = "-lglut" "-lGL" "-lGLU" "-lGLEW"

NAME = "SadBoatEngine-V"
VERSION = "0.02.6V"


prod:
	g++ $(FLAGS) $(PRODUCTION_RELEASE) $(SOURCE_DIR)/*.cpp -I$(INCLUDE_DIR) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)""

test:
	g++ $(FLAGS) $(TEST_RELEASE) $(SOURCE_DIR)/*.cpp -I$(INCLUDE_DIR) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"-T"
	clear

clean:
	rm $(OUT_DIR)/*


