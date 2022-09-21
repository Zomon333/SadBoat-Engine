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
#       mesa-common-dev
#------------------------------------------------------------------------------
#   This package contains:
#      -An Open Source version of OpenGL. 
#
sudo apt-get install mesa-common-dev;

#
#       freeglut3
#------------------------------------------------------------------------------
#   This package contains:
#      -Freeglut: Free Open Source alternative to OpenGL Utility Toolkit.
#
sudo apt-get install freeglut3;

#
#       freeglut3-dev
#------------------------------------------------------------------------------
#   This package contains:
#      -Some Freeglut developer packages for better linux support.
#
sudo apt-get install freeglut3-dev;

