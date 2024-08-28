#
#	Engine Version & Name
#---------------------------------------------------
NAME = "SadBoatEngine-V"
VERSION = "0.03.1V"

#
#	Color Data
#---------------------------------------------------
NONE = "\033[0;0m"
RED = "\033[0;031m"
GREEN = "\033[0;32m"
CYAN = "\033[0;36m"

#
#	General Compilation Info
#---------------------------------------------------
CC = g++
FLAGS = -lstdc++ -std=c++20 -fpermissive -pthread -Wall
DEFINES = -DINCLUDE_VULKAN -DINCLUDE_RAPIDXML

LIBS = -lvulkan -l:libpng16.so.16.39.0 -lz

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

#
#	Resource Location Info
#---------------------------------------------------
SRC_DIR = ./src
OBJ_DIR = ./obj
OUT_DIR = ./output

INCLUDE_DIRS = -I./include/ -I./lib/ -I/usr/include/vulkan/ -I./lib/libpng/include/libpng16 -I./lib/eigen/Eigen
LIBRARY_DIR = -Lusr/lib/x86_64-linux-gnu -L./lib -L./lib/libpng/lib64 -L./lib/vulkan/*/x86_64/lib

LOG_NAME = "compile_log.txt"

game:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(OBJ_DIR)/* $(SRC_DIR)/main.cpp $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)

engine: clean
	make events
	make utilities
	make resources
	make vulkan
	
events:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/events/event.cpp $(LIBS) -o $(OBJ_DIR)/event.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/events/timed_event.cpp $(LIBS) -o $(OBJ_DIR)/timed_event.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/events/recurring_event.cpp $(LIBS) -o $(OBJ_DIR)/recurring_event.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/events/event_pool.cpp $(LIBS) -o $(OBJ_DIR)/event_pool.o

resources: events
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/resources/id_manager.cpp $(LIBS) -o $(OBJ_DIR)/id_manager.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/resources/resource_handle.cpp $(LIBS) -o $(OBJ_DIR)/resource_handle.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/resources/resource_manager.cpp $(LIBS) -o $(OBJ_DIR)/resource_manager.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/resources/resource.cpp $(LIBS) -o $(OBJ_DIR)/resource.o

utilities:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/defines.cpp $(LIBS) -o $(OBJ_DIR)/defines.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/manager.cpp $(LIBS) -o $(OBJ_DIR)/manager.o
	make configs
	make logging

configs:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/configs/config_node.cpp $(LIBS) -o $(OBJ_DIR)/config_node.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/configs/config.cpp $(LIBS) -o $(OBJ_DIR)/config.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/configs/config_manager.cpp $(LIBS) -o $(OBJ_DIR)/config_manager.o

logging:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/logging/log_handle.cpp $(LIBS) -o $(OBJ_DIR)/log_handle.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/logging/log_manager.cpp $(LIBS) -o $(OBJ_DIR)/log_manager.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/logging/startup_logger.cpp $(LIBS) -o $(OBJ_DIR)/startup_logger.o

vulkan:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/vulkan_result_lookup.cpp $(LIBS) -o $(OBJ_DIR)/vulkan_result_lookup.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/physical_device.cpp $(LIBS) -o $(OBJ_DIR)/physical_device.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/physical_device_collection.cpp $(LIBS) -o $(OBJ_DIR)/physical_device_collection.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/extension_collection.cpp $(LIBS) -o $(OBJ_DIR)/extension_collection.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/logical_device.cpp $(LIBS) -o $(OBJ_DIR)/logical_device.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/queue_family.cpp $(LIBS) -o $(OBJ_DIR)/queue_family.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/queue_family_collection.cpp $(LIBS) -o $(OBJ_DIR)/queue_family_collection.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/queue_collection.cpp $(LIBS) -o $(OBJ_DIR)/queue_collection.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/queue.cpp $(LIBS) -o $(OBJ_DIR)/queue.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/command_buffer.cpp $(LIBS) -o $(OBJ_DIR)/command_buffer.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/command_pool_manager.cpp $(LIBS) -o $(OBJ_DIR)/command_pool_manager.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/layer_collection.cpp $(LIBS) -o $(OBJ_DIR)/layer_collection.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/instance.cpp $(LIBS) -o $(OBJ_DIR)/instance.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/image.cpp $(LIBS) -o $(OBJ_DIR)/image.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/buffer.cpp $(LIBS) -o $(OBJ_DIR)/buffer.o
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/command_pool.cpp $(LIBS) -o $(OBJ_DIR)/command_pool.o

clean:
	@echo Cleaning build environment...
	@echo $(OBJ_DIR) $(OUT_DIR)
ifneq ($(strip $(OBJ_DIR)),)
	@echo Deleting contents of $(OBJ_DIR)/
	rm -rf $(OBJ_DIR)/* &
endif
ifneq ($(strip $(OUT_DIR)),)
	@echo Deleting contents of $(OUT_DIR)/
	rm -rf $(OUT_DIR)/* &
endif
