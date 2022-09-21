CC = g++
FLAGS = -std=c++20 -fpermissive

SOURCE_DIR = ./src
INCLUDE_DIR = ./include
LIBRARY_DIR = ./lib
OUT_DIR = ./output

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

LIBS = 

NAME = "SadBoatEngine-V"
VERSION = "0.02.4V"


prod:
	g++ $(FLAGS) $(PRODUCTION_RELEASE) $(SOURCE_DIR)/*.cpp -I$(INCLUDE_DIR) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)""

test:
	g++ $(FLAGS) $(TEST_RELEASE) $(SOURCE_DIR)/*.cpp -I$(INCLUDE_DIR) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"-T"
	clear

clean:
	rm $(OUT_DIR)/*


