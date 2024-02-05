#
#	Engine Version & Name
#---------------------------------------------------
NAME = "SadBoatEngine-V"
VERSION = "0.03.1V"


#
#	General Compilation Info
#---------------------------------------------------
CC = g++
FLAGS = -lstdc++ -std=c++20 -fpermissive -pthread
DEFINES = -DINCLUDE_VULKAN -DINCLUDE_RAPIDXML

LIBS = "-lvulkan" "-l:libpng16.so.16.39.0" "-lz"

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

#
#	Resource Location Info
#---------------------------------------------------
SOURCE_DIR = ./src
OUT_DIR = ./output

INCLUDE_DIRS = "-I./include/" "-I/usr/include/vulkan/" "-I./lib/libpng/include/libpng16" "-I./lib/eigen/Eigen"
LIBRARY_DIR = "-Lusr/lib/x86_64-linux-gnu" "-L./lib" "-L./lib/libpng/lib64" "-L./lib/vulkan/*/x86_64/lib"

LOG_NAME = "compile_log.txt"



#Compiles for production
prod:
	mkdir ./output &
	$(CC) $(FLAGS) $(DEFINES) $(PRODUCTION_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"" 2>&1 | tee $(LOG_NAME)

#Compiles for debug
debug:
	mkdir ./output &
	$(CC) $(FLAGS) $(DEFINES) $(PRODUCTION_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"-dbg" 2>&1 | tee $(LOG_NAME)
	script ./debug_log.txt
	valgrind --leak-check=full --track-origins=yes $(OUT_DIR)/$(NAME)$(VERSION)"-dbg"
	exit

#Compiles with tests
test:
	mkdir ./output &
	$(CC) $(FLAGS) $(DEFINES) $(TEST_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"-T" 2>&1 | tee $(LOG_NAME)

#Removes all built releases
clean:
	rm -rf $(OUT_DIR)/* 	&

libs:
	bash ./get_libs.sh


