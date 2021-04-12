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
	bool Game::keyPressed[348];
	chrono::steady_clock::time_point Game::timePressInit[348];
	Difficulty Game::gameDifficulty;
	float Game::THRESHOLD;

	string Game::getKeyName(int key)
	{
		if (key == -1 || (key > -1 && key < 32))
		{
			return "UNKNOWN";
		}
		if (key == 32)
			return "Space";
		if (key == 39)
			return "Apostrophe";
		if (key == 44)
			return "Comma";
		if (key == 45)
			return "Minus";
		if (key == 46)
			return "Period";
		if (key == 47)
			return "Slash";
		if (key == 48)
			return "0";
		if (key == 49)
			return "1";
		if (key == 50)
			return "2";
		if (key == 51)
			return "3";
		if (key == 52)
			return "4";
		if (key == 53)
			return "5";
		if (key == 54)
			return "6";
		if (key == 55)
			return "7";
		if (key == 56)
			return "8";
		if (key == 57)
			return "9";
		if (key == 59)
			return ";";
		if (key == 61)
			return "=";
		if (key == 65)
			return "A";
		if (key == 66)
			return "B";
		if (key == 67)
			return "C";
		if (key == 68)
			return "D";
		if (key == 69)
			return "E";
		if (key == 70)
			return "F";
		if (key == 71)
			return "G";
		if (key == GLFW_KEY_H)
			return "H";
		if (key == GLFW_KEY_I)
			return "I";
		if (key == GLFW_KEY_J)
			return "J";
		if (key == GLFW_KEY_K)
			return "K";
		if (key == GLFW_KEY_L)
			return "L";
		if (key == GLFW_KEY_M)
			return "M";
		if (key == GLFW_KEY_N)
			return "N";
		if (key == GLFW_KEY_O)
			return "O";
		if (key == GLFW_KEY_P)
			return "P";
		if (key == GLFW_KEY_Q)
			return "Q";
		if (key == GLFW_KEY_R)
			return "R";
		if (key == GLFW_KEY_S)
			return "S";
		if (key == GLFW_KEY_T)
			return "T";
		if (key == GLFW_KEY_U)
			return "U";
		if (key == GLFW_KEY_V)
			return "V";
		if (key == GLFW_KEY_W)
			return "W";
		if (key == GLFW_KEY_X)
			return "X";
		if (key == GLFW_KEY_Y)
			return "Y";
		if (key == GLFW_KEY_Z)
			return "Z";
		if (key == GLFW_KEY_LEFT_BRACKET)
			return "[";
		if (key == GLFW_KEY_BACKSLASH)
			return "BACKSLASH";
		if (key == GLFW_KEY_RIGHT_BRACKET)
			return "]";
		if (key == GLFW_KEY_GRAVE_ACCENT)
			return "`";
		if (key == GLFW_KEY_WORLD_1)
			return "WORLD1";
		if (key == GLFW_KEY_WORLD_2)
			return "WORLD2";

		if (key == GLFW_KEY_ESCAPE)
			return "ESCAPE";
		if (key == GLFW_KEY_ENTER)
			return "ENTER";
		if (key == GLFW_KEY_TAB)
			return "TAB";
		if (key == GLFW_KEY_BACKSPACE)
			return "BACKSPACE";
		if (key == GLFW_KEY_INSERT)
			return "INSERT";
		if (key == GLFW_KEY_DELETE)
			return "DELETE";
		if (key == GLFW_KEY_RIGHT)
			return "RIGHT";
		if (key == GLFW_KEY_LEFT)
			return "LEFT";
		if (key == GLFW_KEY_UP)
			return "UP";
		if (key == GLFW_KEY_DOWN)
			return "DOWN";
		if (key == GLFW_KEY_PAGE_UP)
			return "PAGE UP";
		if (key == GLFW_KEY_PAGE_DOWN)
			return "PAGE DOWN";
		if (key == GLFW_KEY_HOME)
			return "HOME";
		if (key == GLFW_KEY_END)
			return "END";
		if (key == GLFW_KEY_CAPS_LOCK)
			return "CAPS LOCK";
		if (key == GLFW_KEY_SCROLL_LOCK)
			return "SCROLL LOCK";
		if (key == GLFW_KEY_NUM_LOCK)
			return "NUM LOCK";
		if (key == GLFW_KEY_PRINT_SCREEN)
			return "PRINT SCREEN";
		if (key == GLFW_KEY_PAUSE)
			return "PAUSE";
		if (key == GLFW_KEY_F1)
			return "F1";
		if (key == GLFW_KEY_F2)
			return "F2";
		if (key == GLFW_KEY_F3)
			return "F3";
		if (key == GLFW_KEY_F4)
			return "F4";
		if (key == GLFW_KEY_F5)
			return "F5";
		if (key == GLFW_KEY_F6)
			return "F6";
		if (key == GLFW_KEY_F7)
			return "F7";
		if (key == GLFW_KEY_F8)
			return "F8";
		if (key == GLFW_KEY_F9)
			return "F9";
		if (key == GLFW_KEY_F10)
			return "F10";
		if (key == GLFW_KEY_F11)
			return "F11";
		if (key == GLFW_KEY_F12)
			return "F12";
		if (key == GLFW_KEY_F13)
			return "F13";
		if (key == GLFW_KEY_F14)
			return "F14";
		if (key == GLFW_KEY_F15)
			return "F15";
		if (key == GLFW_KEY_F16)
			return "F16";
		if (key == GLFW_KEY_F17)
			return "F17";
		if (key == GLFW_KEY_F18)
			return "F18";
		if (key == GLFW_KEY_F19)
			return "F19";
		if (key == GLFW_KEY_F20)
			return "F20";
		if (key == GLFW_KEY_F21)
			return "F21";
		if (key == GLFW_KEY_F22)
			return "F22";
		if (key == GLFW_KEY_F23)
			return "F23";
		if (key == GLFW_KEY_F24)
			return "F24";
		if (key == GLFW_KEY_F25)
			return "F25";
		if (key == GLFW_KEY_KP_0)
			return "KP0";
		if (key == GLFW_KEY_KP_1)
			return "KP1";
		if (key == GLFW_KEY_KP_2)
			return "KP2";
		if (key == GLFW_KEY_KP_3)
			return "KP3";
		if (key == GLFW_KEY_KP_4)
			return "KP4";
		if (key == GLFW_KEY_KP_5)
			return "KP5";
		if (key == GLFW_KEY_KP_6)
			return "KP6";
		if (key == GLFW_KEY_KP_7)
			return "KP7";
		if (key == GLFW_KEY_KP_8)
			return "KP8";
		if (key == GLFW_KEY_KP_9)
			return "KP9";
		if (key == GLFW_KEY_KP_DECIMAL)
			return "KP DECIMAL";
		if (key == GLFW_KEY_KP_DIVIDE)
			return "KP DIVIDE";
		if (key == GLFW_KEY_KP_MULTIPLY)
			return "KP MULTIPLY";
		if (key == GLFW_KEY_KP_SUBTRACT)
			return "KP SUBTRACT";
		if (key == GLFW_KEY_KP_ADD)
			return "KP ADD";
		if (key == GLFW_KEY_KP_ENTER)
			return "KP ENTER";
		if (key == GLFW_KEY_KP_EQUAL)
			return "KP EQUAL";
		if (key == GLFW_KEY_LEFT_SHIFT)
			return "LEFT SHIFT";
		if (key == GLFW_KEY_LEFT_CONTROL)
			return "LEFT CONTROL";
		if (key == GLFW_KEY_LEFT_ALT)
			return "LEFT ALT";
		if (key == GLFW_KEY_LEFT_SUPER)
			return "LEFT SUPER";
		if (key == GLFW_KEY_RIGHT_SHIFT)
			return "RIGHT SHIFT";
		if (key == GLFW_KEY_RIGHT_CONTROL)
			return "RIGHT CONTROL";
		if (key == GLFW_KEY_RIGHT_ALT)
			return "RIGHT ALT";
		if (key == GLFW_KEY_RIGHT_SUPER)
			return "RIGHT SUPER";
		if (key == GLFW_KEY_MENU)
			return "MENU";


		return "UNKNOWN";
	}

	void Game::error_callback(int error, const char* description)
	{
		cout << "Error: " << glewGetErrorString(error) << ", " << description << endl;
	}

	void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == -1 || key > 348)
			action = GLFW_DONT_CARE;
		if (action == GLFW_PRESS)
		{
			Game::setKey(key, true);
			Game::timePressInit[key] = chrono::steady_clock::now();
		}
		if (action == GLFW_RELEASE)
		{
			Game::setKey(key, false);
		}
	}

	void Game::GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool Game::GLLogCall(const char* function, const char* file, int line)
	{
		GLenum error = glGetError();
		bool hadError = false;
		if (error)
		{
			hadError = true;
		}
		while (error)
		{
			EventParameters GLError;
			GLError.ERROR_ErrorNumber = error;
			GLError.ERROR_EventName = "OpenGLError";
			GLError.ERROR_EventParent = (" file " + (string)(file) + " at line " + to_string(line));
			Event GLErrorEvent = Event(_ERROR, GLError);
			GLErrorEvent.handle();
			GLenum error = glGetError();
		}
		return hadError;
	}

	void Game::setKey(int index, bool state)
	{
		Game::keyPressed[index] = state;
	}

	bool Game::getKey(int index)
	{
		return Game::keyPressed[index];
	}

	void Game::toggleKey(int index)
	{
		if (getKey(index))
		{
			setKey(index, false);
		}
		else
		{
			setKey(index, true);
		}
	}

	chrono::steady_clock::time_point& Game::getTimePress(int key)
	{
		return Game::timePressInit[key];
	}

	chrono::duration<double> Game::getKeyDuration(int key)
	{
		chrono::duration<double> toReturn = (chrono::steady_clock::now() - Game::timePressInit[key]);
		if (toReturn.count() < Game::THRESHOLD)
		{
			toReturn = chrono::seconds(0);
		}
		return toReturn;
	}

	Game::Game(string title)
	{
		cTick = 0;
		tps = 60;
		tick = chrono::milliseconds((1000 / tps));
		ticker = chrono::milliseconds(0);
		deltaT = chrono::steady_clock::now();
		gameStart = chrono::steady_clock::now();
		Game::THRESHOLD = 0.01;
		paused = false;
		running = true;

		gameTitle = title;

		if (Game::gameDifficulty != BEGINNER && Game::gameDifficulty != EASY && Game::gameDifficulty != NORMAL && Game::gameDifficulty != HARD && Game::gameDifficulty != VERYHARD && Game::gameDifficulty != IMPOSSIBLE)
		{
			Game::gameDifficulty = NORMAL;
		}

		if (Game::gameDifficulty == BEGINNER)
		{
			spawnRate = 0.7;
			enemyDamage = 0.5;
			enemyHealth = 0.75;
		}
		if (Game::gameDifficulty == EASY)
		{
			spawnRate = 0.9;
			enemyDamage = 0.8;
			enemyHealth = 0.9;
		}
		if (Game::gameDifficulty == NORMAL)
		{
			spawnRate = 1;
			enemyDamage = 1;
			enemyHealth = 1;
		}
		if (Game::gameDifficulty == HARD)
		{
			spawnRate = 1.1;
			enemyDamage = 1.2;
			enemyHealth = 1.1;
		}
		if (Game::gameDifficulty == VERYHARD)
		{
			spawnRate = 1.5;
			enemyDamage = 1.5;
			enemyHealth = 1.25;
		}
		if (Game::gameDifficulty == IMPOSSIBLE)
		{
			spawnRate = 6;
			enemyDamage = 6;
			enemyHealth = 6;
		}

		int i = 0;
		try
		{
			chrono::duration<double> durationInit = chrono::seconds(0);
			while (i < 348)
			{
				Game::setKey(i, false);
				Game::timePressInit[i] = gameStart;
				i++;
			}
			i = 0;

			int width = 640, height = 480;
			float ratio = (float)(width) / (float)(height);

			if (!glfwInit())
			{
				EventParameters glfwFailInit;
				glfwFailInit.ERROR_ErrorNumber = 2;
				glfwFailInit.ERROR_EventName = "glfwInit()";
				glfwFailInit.ERROR_EventParent = "main()";

				Event glfwFailInitEvent = Event(_ERROR, glfwFailInit);
				glfwFailInitEvent.handle();
				throw glfwFailInitEvent;
			}

			window = glfwCreateWindow(width, height, gameTitle.c_str(), NULL, NULL);
			if (!window)
			{
				EventParameters windowFail;
				windowFail.ERROR_ErrorNumber = 3;
				windowFail.ERROR_EventName = "glfwCreateWindow()";
				windowFail.ERROR_EventParent = "main()";

				Event windowFailEvent = Event(_ERROR, windowFail);
				windowFailEvent.handle();

				glfwTerminate();

				throw windowFailEvent;
			}

			glfwMakeContextCurrent(window);
			glfwSetErrorCallback(&Game::error_callback);
			glfwSetKeyCallback(window, &Game::key_callback);
			glfwSwapInterval(1);
			
			if (glewInit() != GLEW_OK)
			{
				EventParameters glewInitFail;
				glewInitFail.ERROR_ErrorNumber = 4;
				glewInitFail.ERROR_EventName = "glewInit()";
				glewInitFail.ERROR_EventParent = "main()";

				Event glewInitFailEvent = Event(_ERROR, glewInitFail);
				glewInitFailEvent.handle();

				glfwTerminate();

				throw glewInitFailEvent;
			}

		}
		catch (Event e)
		{
			e.handle();
			running = false;
			paused = true;
		}
	}

	void Game::setDifficulty(Difficulty nDifficulty)
	{
		Game::gameDifficulty = nDifficulty;
	}

	Difficulty Game::getDifficulty()
	{
		return Game::gameDifficulty;
	}

	int Game::exec(Game game)
	{
		EventParameters EXIT_PARAMS;
		EXIT_PARAMS.EXIT_reason = "Player requested exit by holding escape for 1 second.";
		Event EXIT_EVENT = Event(EXIT, EXIT_PARAMS);

		bool showConsole = false;

		chrono::steady_clock::time_point lastPause = chrono::steady_clock::now();
		int keyFixer = 0;
		cTick = 0;

		bool Logic, Physics, Collision, Render;
		bool logicJoined, physicsJoined, collisionJoined, renderJoined;

		while (running) 
		{ 
			while (!paused) 
			{ 
				try 
				{
					
					chrono::steady_clock::time_point deltaT = chrono::steady_clock::now();

					if (ticker >= tick)
					{


						Logic = false;
						Physics = false;
						Collision = false;
						Render = false;

						/*logicJoined = false;
						physicsJoined = false;
						collisionJoined = false;
						renderJoined = false;*/

						cTick++;
						ticker = chrono::milliseconds(0);
						

						/*thread gameLogic = thread(Game::gamestate);
						thread gamePhysics = thread(Game::physics);
						thread gameCollision = thread(Game::collision);

						while (!logicJoined || !physicsJoined || !collisionJoined)
						{
							if(gameLogic.joinable() && logicJoined==false)
							{ 
								logicJoined = true;
								gameLogic.join();
								//cout << "logic joined\n";
							}
							if (gamePhysics.joinable() && physicsJoined == false)
							{
								physicsJoined = true;
								gamePhysics.join();
								//cout << "physics joined\n";
							}
							if (gameCollision.joinable() && collisionJoined == false)
							{
								collisionJoined = true;
								gameCollision.join();
								//cout << "collision joined\n";
							}
						}

						thread gameRender = thread(Game::render);

						while (!renderJoined)
						{
							if (gameRender.joinable() && renderJoined == false)
							{
								renderJoined = true;
								gameRender.join();
								//cout << "render joined\n";
							}
						}
						*/
						
				
						if (Game::keyPressed[GLFW_KEY_E])
						{
							EventParameters ERROR_TEST;
							ERROR_TEST.ERROR_ErrorNumber = 1;
							ERROR_TEST.ERROR_EventName = "Game::exec()";
							ERROR_TEST.ERROR_EventParent = "Game::exec()";
							Event ERROR_EVENT = Event(_ERROR, ERROR_TEST);
							throw ERROR_EVENT;
						}


						if (Game::getKeyDuration(GLFW_KEY_ESCAPE).count() > 1 && Game::getKey(GLFW_KEY_ESCAPE))
							throw EXIT_EVENT;


						if (cTick == (tps - 1))
						{
							cTick = 0;
						}
					}

					glfwPollEvents();

					keyFixer = 0;
					while(keyFixer<348)
					{
						if (!getKey(keyFixer))
						{
							Game::timePressInit[keyFixer] = chrono::steady_clock::now();
						}
						else
						{
							cout.width(2);
							cout.precision(3);
							cout << " " << Game::getKeyName(keyFixer) << ": " << Game::getKeyDuration(keyFixer).count() << "	";
						}
						keyFixer++;
					}
					keyFixer = 0;
					cout << endl;
					ticker += (chrono::steady_clock::now() - deltaT);
		}
		catch (Event &e)
		{
			if (e.getType() == EXIT)
			{
				cout << "EXIT_EVENT THROWN\nReason: " << e.getParams().EXIT_reason << endl;
				running = false;
				paused = true;
				glfwTerminate();
				return -1;
			}

			if (e.getType() == _ERROR)
			{
				thread eventThread = thread(&Event::handle, e);
				while (!eventThread.joinable());
				eventThread.join();
				running = false;
				paused = true;
				glfwTerminate();
				return e.getParams().ERROR_ErrorNumber;
			}

			if (e.getType() != _ERROR && e.getType() != EXIT)
			{
				thread eventThread = thread(&Event::handle, e);
				while (!eventThread.joinable());
				eventThread.join();
			}
		}

	}
}}

	int Game::gamestate()
	{
		return 0;
	}

	int Game::physics()
	{
		return 0;
	}

	int Game::collision()
	{
		return 0;
	}

	int Game::render()
	{
		return 0;
	}




}