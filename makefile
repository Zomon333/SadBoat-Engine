CC = g++
FLAGS = -std=c++20 -ffreestanding -fpermissive -pthread

BR = "\033[1;36m---------------------------------------------------\033[0m"

DEFINES = -DINCLUDE_VULKAN -DINCLUDE_RAPIDXML

SOURCE_DIR = ./src
INCLUDE_DIRS = "-I./include/" "-I/usr/include/vulkan/"
OUT_DIR = ./output
LIBRARY_DIR = "usr/lib/x86_64-linux-gnu" "./lib"

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

LIBS = "-lvulkan"
MESA_V = "mesa-22.2.0"

NAME = "SadBoatEngine-V"
VERSION = "0.02.7V"


prod:
	g++ $(FLAGS) $(DEFINES) $(PRODUCTION_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)""

test:
	g++ $(FLAGS) $(DEFINES) $(TEST_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"-T"

clean:
	rm -rf $(OUT_DIR)/* 	&
	rm -rf ./lib/mesa 		&

vulkan_install:
	@rm -rf ./lib/mesa &
	@echo "\033[1;36mInstalling Vulkan Compilation Tools...\033[0m"
	@echo $(BR);

	sudo apt install meson
	sudo apt install ninja-build
	sudo apt install glslang-tools
	
	@echo "\n\n"
	@echo "\033[1;36mDownloading mesa\033[0m"
	@echo $(BR)

	mkdir ./lib/mesa
	
	wget https://archive.mesa3d.org//$(MESA_V).tar.xz
	mv $(MESA_V).tar.xz ./lib/mesa

	@echo "\n\n"
	@echo "\033[1;36mUnpacking mesa\033[0m"
	@echo $(BR)
	
	tar --extract -f ./lib/mesa/$(MESA_V).tar.xz 
	mv -f ./$(MESA_V) ./lib/mesa/

	mkdir ./lib/mesa/$(MESA_V)/build
	
	@echo "\n\n"
	@echo "\033[1;36mCreating ninja config files\033[0m"
	@echo $(BR)

	meson ./lib/mesa/$(MESA_V) ./lib/mesa/$(MESA_V)/build

	@echo "\n\n"
	@echo "\033[1;36mCompiling mesa with ninja\033[0m"
	@echo $(BR)

	ninja -C ./lib/mesa/$(MESA_V)/build

	@echo "\n\n"
	@echo "\033[1;36mInstalling mesa\033[0m"
	@echo $(BR)

	sudo apt-get install mesa
	sudo apt-get build-dep mesa

	sudo ninja -C ./lib/mesa/$(MESA_V)/build install

	@echo "\n\n"
	@echo "\033[1;36mInstalling libvulkan-dev and vulkan tools\033[0m"
	@echo $(BR)

	sudo apt-get install libvulkan-dev
	sudo apt install vulkan-tools vulkan-validationlayers-dev spirv-tools

