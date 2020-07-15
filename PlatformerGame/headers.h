#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <intrin.h>
#include <windows.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) PPS::Game::GLClearError();\
	x;\
	ASSERT(PPS::Game::GLLogCall(#x, __FILE__, __LINE__))


// Contains declaration of Difficulty enumerator
#include "Difficulty.h"

// Contains declaration of point class & declaration of point-related member functions
// Point.cpp contains definition of said functions.
#include "Point.h"

// Contains declaration of line class & declaration of line-related member functions
// Line.cpp contains definition of said functions.
#include "Line.h"

// Contains declaration of the range system: A rudimentary greater than/less than system overloaded to support operators.
// range.cpp contains definition of said operators.
#include "range.h"

// Contains declaration of bounding box system: A rudimentary rectangle made of ranges, which can be tested to see if you're within.
// BoundingBox.cpp contains definition of the testing operators.
#include "BoundingBox.h"

// Contains declaration of vertex buffer class & declaration of line-related member functions
// VertexBuffer.cpp contains definition of said functions.
#include "VertexBuffer.h"

//Contains declaration of shader class & declaration of shader-related member functions.
//Shader.cpp contains definition of said functions.
#include "Shader.h" 

//Contains declaration of Event class, eventParameters class, eventType enumerator, and generic event handling functions.
//Event.cpp contains definitions for the generic event handling functions. Event::handle is virtual and will be redefined by individual objects who inherit Event.
#include "Event.h"

//Contains declaration of main game loop, OpenGL startup, keybindings, and multithreading handler.
//Game.cpp contains definition of these things, although most of them are through the Game::Game() constructor, which also initializes the exec loop.
#include "Game.h"


static HWND Console;