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
#       Vulkan:
#           -Mesa3D: Vulkan
#               -Meson
#               -Ninja
#               -GLSLang Tools
#           -libvulkan-dev
#           -vulkan-tools
#           -vulkan-validationlayers-dev
#           -spirv-tools
#------------------------------------------------------------------------------
#   This contains:
#      -Mesa3D open source graphics library.
#      -Mesa3D compilation tools
#      -Development headers for vulkan
#      -Tools to test your vulkan environment
#      -Debugging layers for your vulkan envionrment
#      -Required SPIR-V tools
#
make clean
make vulkan_install




