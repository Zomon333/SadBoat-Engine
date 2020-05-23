#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <string>
#include <iostream>
#include "keybinds.h"
#include <thread>

//Globals


//Function prototypes
void error_callback(int error, const char* description);
void physics();

//#include <stddef.h>
//#include <Windows.h>
//We'll need the header file above if we want Sleep();