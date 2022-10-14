#!/bin/bash

#
#      install_libs.sh
#
#   This script is designed to install the essential libraries we use in
#   the SadBoat engine. Much like the makefile, it's intended for use on
#   linux. (Ubuntu 22.04 / Some Debian) Use with caution!
#------------------------------------------------------------------------------      
#

#
#       Mesa
#------------------------------------------------------------------------------
#   This package contains:
#      -Mesa3D open source graphics library.
#

sudo apt-get install mesa                               #Get the most recent Mesa
sudo apt-get build-dep mesa                             #Update and get it's dependencies

sudo apt install meson					#Meson build configurer
sudo apt install ninja-build				#Ninja build tool install
sudo apt install glslang-tools				#glslang dependency

mkdir local-mesa                                        #Make a folder called local-mesa
cd ./local-mesa                                         #Move into local-mesa

wget https://archive.mesa3d.org//mesa-22.2.0.tar.xz     #Download our version of Mesa
tar -xf ./mesa-22.2.0.tar.xz                            #Unzip it
cd ./mesa-22.2.0.tar.xz                                 #Move into out version of Mesa

mkdir build                                             #Make a build directory

meson ./ ./build                                        #Generate ninja build files for that directory
ninja -C ./build                                        #Build that directory
sudo ninja -C ./build install                           #Install the built files

cd ../..



#
#       libvulkan-dev
#------------------------------------------------------------------------------
#   This package contains:
#      -Development headers for vulkan. 
#
sudo apt-get install libvulkan-dev


#
#       vulkan-tools, vulkan-validationlayers-dev, spirv-tools
#------------------------------------------------------------------------------
#   This package contains:
#      -Tools to test your vulkan environment.
#      -Debugging layers to test your vulkan development.
#      -Required SPIR-V tools.
#
sudo apt install vulkan-tools vulkan-validationlayers-dev spirv-tools



