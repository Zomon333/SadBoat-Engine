#pragma once
#include "headers.h"
using namespace std;
namespace PPS
{
	static class Game
	{
	private:
		chrono::steady_clock::time_point gameStart;
		static Difficulty gameDifficulty;

		float spawnRate;
		float enemyHealth;
		float enemyDamage;

		int cTick;
		int tps;
		chrono::duration<double, std::milli> tick;
		chrono::steady_clock::duration ticker;
		chrono::steady_clock::time_point deltaT;
		static float THRESHOLD;

		bool paused;
		bool running;


		static bool keyPressed[348];
		static chrono::steady_clock::time_point timePressInit[348];
		//static Point cursorPos;

		
	public:
		Game();
		GLFWwindow* window;

		static void setDifficulty(Difficulty nDifficulty);
		static Difficulty getDifficulty();

		static void setKey(int index, bool state);
		static bool getKey(int index);
		static void toggleKey(int index);
		
		//Implemented but did not solve problems
		static chrono::steady_clock::time_point& getTimePress(int key); //Get when the key was depressed
		static chrono::duration<double> getKeyDuration(int key); //Get how long a key has been held

		string getKeyName(int key);

		static void error_callback(int error, const char* description);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void GLClearError();
		static bool GLLogCall(const char* function, const char* file, int line);

		int exec(Game game);

		static int gamestate();
		static int physics();
		static int collision();
		static int render();

	};
}
