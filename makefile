#
#	Engine Version & Name
#---------------------------------------------------
NAME = "SadBoatEngine-V"
VERSION = "0.03.1V"


#
#	General Compilation Info
#---------------------------------------------------
CC = g++
FLAGS = -std=c++20 -ffreestanding -fpermissive -pthread
DEFINES = -DINCLUDE_VULKAN -DINCLUDE_RAPIDXML

LIBS = "-lvulkan" "-l:libpng16.so.16.39.0" "-lz"

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

#
#	Resource Location Info
#---------------------------------------------------
SOURCE_DIR = ./src
OUT_DIR = ./output

INCLUDE_DIRS = "-I./include/" "-I/usr/include/vulkan/" "-I./lib/libpng/include/libpng16"
LIBRARY_DIR = "-Lusr/lib/x86_64-linux-gnu" "-L./lib" "-L./lib/libpng/lib64"





#Compiles for production
prod:
	mkdir ./output &
	g++ $(FLAGS) $(DEFINES) $(PRODUCTION_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)""

#Compiles for debug
debug:
	mkdir ./output &
	g++ $(FLAGS) $(DEFINES) $(PRODUCTION_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"-dbg"
	script ./debug_log.txt
	valgrind --leak-check=full --track-origins=yes $(OUT_DIR)/$(NAME)$(VERSION)"-dbg"
	exit

#Compiles with tests
test:
	mkdir ./output &
	g++ $(FLAGS) $(DEFINES) $(TEST_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"-T"

#Removes all built releases
clean:
	rm -rf $(OUT_DIR)/* 	&

#Install the Vulkan SDK
vulkan_lunarg:
	yes | rm -r ./lib/vulkan/* &
	curl "https://sdk.lunarg.com/sdk/download/latest/linux/vulkan-sdk.tar.gz" --output ./lib/vulkan/vulkan-sdk.tar.gz
	tar --extract -f ./lib/vulkan/vulkan-sdk.tar.gz -C ./lib/vulkan
	./lib/vulkan/*/vulkansdk


