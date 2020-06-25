#pragma once
#include "headers.h"
using namespace std;

namespace PPS
{
	bool Game::keyPressed[348];
	chrono::steady_clock::time_point Game::timePressInit[348];
	chrono::duration<double> Game::pressDuration[348];

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
			Game::setTimePress(key, chrono::steady_clock::now());
		}
		if (action == GLFW_RELEASE)
		{
			Game::setKey(key, false);
		}

		if (Game::getKey(key))
		{
			Game::setKeyDuration(key, (chrono::steady_clock::now() - Game::timePressInit[key]));
		}
		else
		{
			Game::setTimePress(key, chrono::steady_clock::now());
			Game::setKeyDuration(key, chrono::milliseconds(0));
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
			Event GLErrorEvent = Event(ERROR, GLError);
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

	void Game::setTimePress(int key, chrono::steady_clock::time_point nTime)
	{
		Game::timePressInit[key] = nTime;
	}

	chrono::duration<double>& Game::getKeyDuration(int key)
	{
		return Game::pressDuration[key];
	}

	void Game::setKeyDuration(int key, chrono::duration<double> pressDuration)
	{
		Game::pressDuration[key] = pressDuration;
	}

	Game::Game()
	{
		cTick = 0;
		tps = 60;
		tick = chrono::milliseconds((1000 / tps));
		ticker = chrono::milliseconds(0);
		deltaT = chrono::steady_clock::now();
		gameStart = chrono::steady_clock::now();
		paused = false;
		running = true;


		int i = 0;
		try
		{
			chrono::duration<double> durationInit = chrono::seconds(0);
			while (i < 348)
			{
				Game::setKey(i, false);
				Game::setTimePress(i, gameStart);
				Game::setKeyDuration(i, durationInit);
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

				Event glfwFailInitEvent = Event(ERROR, glfwFailInit);
				glfwFailInitEvent.handle();
				throw glfwFailInitEvent;
			}

			window = glfwCreateWindow(width, height, "Bark", NULL, NULL);
			if (!window)
			{
				EventParameters windowFail;
				windowFail.ERROR_ErrorNumber = 3;
				windowFail.ERROR_EventName = "glfwCreateWindow()";
				windowFail.ERROR_EventParent = "main()";

				Event windowFailEvent = Event(ERROR, windowFail);
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

				Event glewInitFailEvent = Event(ERROR, glewInitFail);
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

	int Game::exec()
	{
		EventParameters EXIT_PARAMS;
		EXIT_PARAMS.EXIT_reason = "Player requested exit by holding escape for 1 second.";
		Event EXIT_EVENT = Event(EXIT, EXIT_PARAMS);

		chrono::steady_clock::time_point lastPause = chrono::steady_clock::now();

		cTick = 0;
		while (running) { while (!paused) { try {
					
			chrono::steady_clock::time_point deltaT = chrono::steady_clock::now();

			if (ticker >= tick)
			{
				cTick++;
				ticker = chrono::milliseconds(0);

				cout << "Space: "<< Game::keyPressed[GLFW_KEY_SPACE] << endl;
				cout << "A: " << Game::keyPressed[GLFW_KEY_A] << endl;

				if (Game::keyPressed[GLFW_KEY_E])
				{
					EventParameters ERROR_TEST;
					ERROR_TEST.ERROR_ErrorNumber = 1;
					ERROR_TEST.ERROR_EventName = "Game::exec()";
					ERROR_TEST.ERROR_EventParent = "Game::exec()";
					Event ERROR_EVENT = Event(ERROR, ERROR_TEST);
					throw ERROR_EVENT;
				}

				if (Game::pressDuration[GLFW_KEY_ESCAPE] > chrono::seconds(1))
					throw EXIT_EVENT;


				if (cTick == (tps - 1))
				{
					cTick = 0;
				}
			}

			glfwPollEvents();
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

			if (e.getType() == ERROR)
			{
				e.handle();
				running = false;
				paused = true;
				glfwTerminate();
				return e.getParams().ERROR_ErrorNumber;
			}
		}
	}
}}




}