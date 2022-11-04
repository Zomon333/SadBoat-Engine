#
#	Engine Version & Name
#---------------------------------------------------
NAME = "SadBoatEngine-V"
VERSION = "0.02.7V"


#
#	General Compilation Info
#---------------------------------------------------
CC = g++
FLAGS = -std=c++20 -ffreestanding -fpermissive -pthread
DEFINES = -DINCLUDE_VULKAN -DINCLUDE_RAPIDXML

LIBS = "-lvulkan"

PRODUCTION_RELEASE = -DCONFIG_PROD
TEST_RELEASE = -DCONFIG_TEST

#
#	Resource Location Info
#---------------------------------------------------
SOURCE_DIR = ./src
OUT_DIR = ./output

INCLUDE_DIRS = "-I./include/" "-I/usr/include/vulkan/"
LIBRARY_DIR = "-Lusr/lib/x86_64-linux-gnu" "-L./lib"


#
#	Mesa Formatting Info
#---------------------------------------------------
MESA_V = "mesa-22.2.0"
NOBUILD = -Dlibunwind=disabled
BR = "\033[1;36m---------------------------------------------------\033[0m"




#Compiles for production
prod:
	g++ $(FLAGS) $(DEFINES) $(PRODUCTION_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) -L$(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)""

#Compiles with tests
test:
	g++ $(FLAGS) $(DEFINES) $(TEST_RELEASE) $(SOURCE_DIR)/*.cpp $(INCLUDE_DIRS) $(LIBRARY_DIR) $(LIBS) -o $(OUT_DIR)/$(NAME)$(VERSION)"-T"

#Removes all built releases
clean:
	rm -rf $(OUT_DIR)/* 	&

#Installs the tools needed to build vulkan
vulkan_build_tools:
	@echo "\033[1;36mInstalling Vulkan Compilation Tools...\033[0m"
	@echo $(BR);
	
	sudo apt-get install -y g++
	
	sudo apt-get install -y cmake
	sudo apt-get install -y bison
	sudo apt-get install -y byacc
	sudo apt-get install -y flex

	sudo apt install -y meson
	sudo apt install -y ninja-build
	sudo apt install -y glslang-tools
	sudo apt-get install -y python3	
	sudo apt-get install -y pip
	pip install mako

#Attempts to install vulkan's dependencies. No guarantees this will work with your system!
vulkan_deps:
	@echo "\033[1;36mInstalling Vulkan Dependencies...\033[0m"
	@echo $(BR);

	sudo apt-get install -y libudev-dev llvm libelf-dev	
	sudo apt-get install -y libwayland-dev libwayland-egl-backend-dev
	sudo apt-get install -y libx11-dev libxext-dev libxfixes-dev
	sudo apt-get install -y libxcb-glx0-dev libxcb-shm0-dev libx11-xcb-dev
	sudo apt-get install -y libxcb-dri2-0 libxcb-dri2-0-dev libxcb-dri3-0 libxcb-dri3-dev
	sudo apt-get install -y libxcb-present-dev libx11-dev libpthread-stubs0-dev libudev-dev libpciaccess-dev
	sudo apt-get install -y libxshmfence-dev libxshmfence1
	sudo apt-get install -y libxxf86vm-dev libxxf86vm1 libxxf86vm1-dbg
	sudo apt-get install -y x11-xserver-utils libxrandr-dev libxrandr2 lxde 
	

	@echo "\n\n"
	@echo "\033[1;36mInstalling Vulkan Dependency Dependencies\033[0m"
	@echo $(BR);
	sudo apt-get install -y libxml2-dev doxygen xsltproc xmlto

	rm -rf ./lib/drm
	
	@echo "\n\n"
	@echo "\033[1;36mCloning libdrm\033[0m"
	@echo $(BR);
	git clone git://anongit.freedesktop.org/git/mesa/drm ./lib/drm
	mkdir ./lib/drm/build
	
	@echo "\n\n"
	@echo "\033[1;36mConfiguring libdrm\033[0m"
	@echo $(BR);
	meson ./lib/drm ./lib/drm/build
	
	@echo "\n\n"
	@echo "\033[1;36mBuilding libdrm\033[0m"
	@echo $(BR);
	ninja -C ./lib/drm/build
	
	@echo "\n\n"
	@echo "\033[1;36mInstalling libdrm\033[0m"
	@echo $(BR);
	sudo ninja -C ./lib/drm/build install


	rm -rf ./lib/wayland
	
	@echo "\n\n"
	@echo "\033[1;36mCloning wayland\033[0m"
	@echo $(BR);
	git clone https://gitlab.freedesktop.org/wayland/wayland.git ./lib/wayland
	mkdir ./lib/wayland/build
	
	@echo "\n\n"
	@echo "\033[1;36mConfiguring wayland\033[0m"
	@echo $(BR);
	meson ./lib/wayland ./lib/wayland/build
	
	@echo "\n\n"
	@echo "\033[1;36mBuilding wayland\033[0m"
	@echo $(BR);
	ninja -C ./lib/wayland/build
	
	@echo "\n\n"
	@echo "\033[1;36mInstalling wayland\033[0m"
	@echo $(BR);
	sudo ninja -C ./lib/wayland/build install




	rm -rf ./lib/wayland-protocols
	
	@echo "\n\n"
	@echo "\033[1;36mCloning wayland-protocols\033[0m"
	@echo $(BR);
	git clone https://gitlab.freedesktop.org/wayland/wayland-protocols.git ./lib/wayland-protocols
	mkdir ./lib/wayland-protocols/build
	
	@echo "\n\n"
	@echo "\033[1;36mConfiguring wayland-protocols\033[0m"
	@echo $(BR);
	meson ./lib/wayland-protocols ./lib/wayland-protocols/build
	
	@echo "\n\n"
	@echo "\033[1;36mBuilding wayland-protocols\033[0m"
	@echo $(BR);
	ninja -C ./lib/wayland-protocols/build
	
	@echo "\n\n"
	@echo "\033[1;36mInstalling wayland-protocols\033[0m"
	@echo $(BR);
	sudo ninja -C ./lib/wayland-protocols/build install
	
	

	

#Creates a fresh Vulkan install
#May require some alternate dependencies that are not listed
vulkan_install:
	@./confirm.sh "Mesa3D requires your apt and apt-get settings to allow source code downloads. Are these enabled?"

	@rm -rf ./lib/mesa
	
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

	meson $(NOBUILD) ./lib/mesa/$(MESA_V) ./lib/mesa/$(MESA_V)/build

	@echo "\n\n"
	@echo "\033[1;36mCompiling mesa with ninja\033[0m"
	@echo $(BR)

	ninja -C ./lib/mesa/$(MESA_V)/build

	@echo "\n\n"
	@echo "\033[1;36mInstalling mesa\033[0m"
	@echo $(BR)

	sudo ninja -C ./lib/mesa/$(MESA_V)/build install

	@echo "\n\n"
	@echo "\033[1;36mInstalling libvulkan-dev and vulkan tools\033[0m"
	@echo $(BR)

	sudo apt-get install -y libvulkan-dev
	sudo apt install -y vulkan-tools vulkan-validationlayers-dev spirv-tools

	@echo "\n\n"
	@echo "\033[1;36mDeleting $(MESA_V).tar.xz.1\033[0m"
	@echo $(BR)
	rm -rf ./lib/mesa/$(MESA_V).tar.xz.1 &





