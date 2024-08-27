#
#	Engine Version & Name
#---------------------------------------------------
NAME = "SadBoatEngine-V"
VERSION = "0.03.1V"


#
#	General Compilation Info
#---------------------------------------------------
CC = g++
FLAGS = -lstdc++ -std=c++20 -fpermissive -pthread -Wall
DEFINES = -DINCLUDE_VULKAN -DINCLUDE_RAPIDXML

LIBS = "-lvulkan" "-l:libpng16.so.16.39.0" "-lz"

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

#
#	Resource Location Info
#---------------------------------------------------
SRC_DIR = ./src
OBJ_DIR = ./obj
OUT_DIR = ./output

INCLUDE_DIRS = "-I./include/" "-I./lib/" "-I/usr/include/vulkan/" "-I./lib/libpng/include/libpng16" "-I./lib/eigen/Eigen"
LIBRARY_DIR = "-Lusr/lib/x86_64-linux-gnu" "-L./lib" "-L./lib/libpng/lib64" "-L./lib/vulkan/*/x86_64/lib"

LOG_NAME = "compile_log.txt"

game:
	make engine
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(OBJ_DIR)/* $(SRC_DIR)/main.cpp -o $(OUT_DIR)/$(NAME)$(VERSION)

engine:
	make clean
	make events
	make utilities
	make resources
	make vulkan
	
events:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/events/event.cpp -o $(OBJ_DIR)/event.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/events/timed_event.cpp -o $(OBJ_DIR)/timed_event.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/events/recurring_event.cpp -o $(OBJ_DIR)/recurring_event.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/events/event_pool.cpp -o $(OBJ_DIR)/event_pool.o

resources:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/resources/id_manager.cpp -o $(OBJ_DIR)/id_manager.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/resources/resource_handle.cpp -o $(OBJ_DIR)/resource_handle.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/resources/resource_manager.cpp -o $(OBJ_DIR)/resource_manager.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/resources/resource.cpp -o $(OBJ_DIR)/resource.o

utilities:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/utilities/defines.cpp -o $(OBJ_DIR)/defines.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/utilities/manager.cpp -o $(OBJ_DIR)/manager.o
	make configs
	make logging

configs:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/utilities/configs/config_node.cpp -o $(OBJ_DIR)/config_node.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/utilities/configs/config.cpp -o $(OBJ_DIR)/config.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/utilities/configs/config_manager.cpp -o $(OBJ_DIR)/config_manager.o

logging:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/utilities/logging/log_handle.cpp -o $(OBJ_DIR)/log_handle.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/utilities/logging/log_manager.cpp -o $(OBJ_DIR)/log_manager.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -c $(SRC_DIR)/utilities/logging/startup_logger.cpp -o $(OBJ_DIR)/startup_logger.o

vulkan:

# Order to compile:
# instance
# physicaldevice
# physicaldevicecollection
# extensioncollection
# layercollection
# logicaldevice
# queuefamily
# queuefamilycollection
# queuecollection
# queue
# commandbuffer
# commandpool
# commandpoolmanager
# buffer
# image

clean:
	rm -rf $(OBJ_DIR)/* $(OUT_DIR)/* 	&