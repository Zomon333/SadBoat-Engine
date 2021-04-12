#pragma once
#include "headers.h"
/*
Dagan Poulin, SadBoat Entertainment, 4/12/2021
 Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
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

		string gameTitle;

		static bool keyPressed[348];
		static chrono::steady_clock::time_point timePressInit[348];
		static Point cursorPos;
		
		
	public:
		Game(string title);
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
