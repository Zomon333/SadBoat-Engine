#pragma once
#include "headers.h"
using namespace std;
namespace PPS
{
	static class Game
	{
	private:
		chrono::steady_clock::time_point gameStart;
		

		int cTick;
		int tps;
		chrono::duration<double, std::milli> tick;
		chrono::steady_clock::duration ticker;
		chrono::steady_clock::time_point deltaT;

		bool paused;
		bool running;


		static bool keyPressed[348];
		static chrono::steady_clock::time_point timePressInit[348];
		static chrono::duration<double> pressDuration[348];
		//static Point cursorPos;

		
	public:
		Game();
		GLFWwindow* window;


		static void setKey(int index, bool state);
		static bool getKey(int index);
		static void toggleKey(int index);
		
		
		//Implemented but did not solve problems
		static chrono::steady_clock::time_point& getTimePress(int key); //Get when the key was depressed
		static void setTimePress(int key, chrono::steady_clock::time_point nTime); //Set when the key was depressed

		static chrono::duration<double>& getKeyDuration(int key); //Get how long a key has been held
		static void setKeyDuration(int key, chrono::duration<double> pressDuration); //Set how long a key has been held
		

		static void error_callback(int error, const char* description);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		int exec();

	};
}
