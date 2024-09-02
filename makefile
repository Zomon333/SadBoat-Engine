#
#	Engine Version & Name
#---------------------------------------------------
NAME = "SadBoatEngine-V"
VERSION = "0.03.1"

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

LIBS = -lvulkan -l:libpng16.so.16.39.0 -lz -lglfw3

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

#
#	Resource Location Info
#---------------------------------------------------
SRC_DIR = ./src
OBJ_DIR = ./obj
OUT_DIR = ./output

INCLUDE_DIRS = -I./include/ -I./lib/ -I/usr/include/vulkan/ -I./lib/libpng/include/libpng16 -I./lib/eigen/Eigen -I./lib/glfw/include/GLFW/
LIBRARY_DIR = -L/usr/lib/x86_64-linux-gnu -L./lib -L./lib/libpng/lib64 -L./lib/vulkan/*/x86_64/lib -L./lib/glfw/build/src

LOG_NAME = "compile_log.txt"

.PHONY: all clean

all:
	${MAKE} -j32 clean engine game

game: vulkan_env
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) $(OBJ_DIR)/* $(SRC_DIR)/main.cpp $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)

# COMPLETE ENGINE
engine: clean events utilities resources vulkan vulkan_env
	@echo "Finished building engine."
	

# EVENTS SYSTEM
events: _event _timed_event _recurring_event _event_pool
_event:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/events/event.cpp $(LIBS) -o $(OBJ_DIR)/event.o

_timed_event:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/events/timed_event.cpp $(LIBS) -o $(OBJ_DIR)/timed_event.o

_recurring_event:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/events/recurring_event.cpp $(LIBS) -o $(OBJ_DIR)/recurring_event.o

_event_pool:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/events/event_pool.cpp $(LIBS) -o $(OBJ_DIR)/event_pool.o


# RESOURCE MANAGEMENT SYSTEM
resources: events _id_manager _resource_handle _resource_manager _resource
_id_manager:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/resources/id_manager.cpp $(LIBS) -o $(OBJ_DIR)/id_manager.o
_resource_handle:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/resources/resource_handle.cpp $(LIBS) -o $(OBJ_DIR)/resource_handle.o
_resource_manager:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/resources/resource_manager.cpp $(LIBS) -o $(OBJ_DIR)/resource_manager.o
_resource:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/resources/resource.cpp $(LIBS) -o $(OBJ_DIR)/resource.o


# GENERAL UTILITIES
utilities: configs logging _defines _manager
_defines:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/defines.cpp $(LIBS) -o $(OBJ_DIR)/defines.o
_manager:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/manager.cpp $(LIBS) -o $(OBJ_DIR)/manager.o


# CONFIGURATION SYSTEM
configs: _config_node _config _config_manager
_config_node:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/configs/config_node.cpp $(LIBS) -o $(OBJ_DIR)/config_node.o
_config:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/configs/config.cpp $(LIBS) -o $(OBJ_DIR)/config.o
_config_manager:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/configs/config_manager.cpp $(LIBS) -o $(OBJ_DIR)/config_manager.o


# LOGGING SYSTEM
logging: _log_handle _log_manager _startup_logger
_log_handle: 
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/logging/log_handle.cpp $(LIBS) -o $(OBJ_DIR)/log_handle.o
_log_manager: 
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/logging/log_manager.cpp $(LIBS) -o $(OBJ_DIR)/log_manager.o
_startup_logger:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/utilities/logging/startup_logger.cpp $(LIBS) -o $(OBJ_DIR)/startup_logger.o

# VULKAN_ENVIRONMENT RENDERING SYSTEM
vulkan_env:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/vulkan_environment.cpp $(LIBS) -o $(OBJ_DIR)/vulkan_environment.o


# VULKAN SOURCE FILES
vulkan: _vulkan_result_lookup _physical_device _physical_device_collection _extension_collection _logical_device _queue_family _queue_family_collection _queue_collection _queue _command_buffer _command_pool_manager _layer_collection _instance _image _buffer _command_pool _swapchain _renderpass
_vulkan_result_lookup:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/vulkan_result_lookup.cpp $(LIBS) -o $(OBJ_DIR)/vulkan_result_lookup.o
_physical_device:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/physical_device.cpp $(LIBS) -o $(OBJ_DIR)/physical_device.o
_physical_device_collection:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/physical_device_collection.cpp $(LIBS) -o $(OBJ_DIR)/physical_device_collection.o
_extension_collection:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/extension_collection.cpp $(LIBS) -o $(OBJ_DIR)/extension_collection.o
_logical_device:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/logical_device.cpp $(LIBS) -o $(OBJ_DIR)/logical_device.o
_queue_family:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/queue_family.cpp $(LIBS) -o $(OBJ_DIR)/queue_family.o
_queue_family_collection:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/queue_family_collection.cpp $(LIBS) -o $(OBJ_DIR)/queue_family_collection.o
_queue_collection:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/queue_collection.cpp $(LIBS) -o $(OBJ_DIR)/queue_collection.o
_queue:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/queue.cpp $(LIBS) -o $(OBJ_DIR)/queue.o
_command_buffer:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/command_buffer.cpp $(LIBS) -o $(OBJ_DIR)/command_buffer.o
_command_pool_manager:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/command_pool_manager.cpp $(LIBS) -o $(OBJ_DIR)/command_pool_manager.o
_layer_collection:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/layer_collection.cpp $(LIBS) -o $(OBJ_DIR)/layer_collection.o
_instance:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/instance.cpp $(LIBS) -o $(OBJ_DIR)/instance.o
_image:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/image.cpp $(LIBS) -o $(OBJ_DIR)/image.o
_buffer:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/buffer.cpp $(LIBS) -o $(OBJ_DIR)/buffer.o
_command_pool:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/command_pool.cpp $(LIBS) -o $(OBJ_DIR)/command_pool.o
_swapchain:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/swapchain.cpp $(LIBS) -o $(OBJ_DIR)/swapchain.o
_renderpass:
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIR) -c $(SRC_DIR)/vulkan/renderpass.cpp $(LIBS) -o $(OBJ_DIR)/renderpass.o


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
