#pragma once
#include "headers.h"
using namespace std;

#pragma region Constants
static float x = 0, y = 0, xAccel = 0, yAccel = 0;
static int frames = 0;
static float fps = 0;
static double fpsOldTime = 0;
static double fpsNewTime = 0;
static double dT = 0;
static keybibbles keypress;
static unsigned int buffer; //Create an unsigned int to be used as a buffer
#pragma endregion Constants

void physics()
{
	float dilution = 0.05;
	bool oldPhysics = true;
	fpsNewTime = glfwGetTime();
	if (fpsNewTime >= (fpsOldTime + 2))
	{
		fps = frames / (fpsNewTime - fpsOldTime);
		cout << (fpsNewTime - fpsOldTime) << " seconds, causing " << fps << " FPS\n";
		fpsOldTime = fpsNewTime;
		frames = 0;
		//Sleep(1500);
	}
	dT = (fpsNewTime - fpsOldTime) / 1;
	if (!oldPhysics)
	{
		cout << "X: " << to_string(x) << " , Y: " << y << " xAccel: " << xAccel << " , yAccel: " << yAccel << endl; //Movement & keybind stuff
		if (y > 0)
		{
			if (yAccel > 0)
			{
				yAccel -= .98;
			}
			else
			{
				yAccel = -.98;
			}
			if (y < 4)
			{
				if (xAccel > 0.5)
				{
					xAccel -= 0.5;
				}
				if (xAccel < -0.5)
				{
					xAccel += 0.5;
				}
				if (xAccel > -0.5 && xAccel < 0.5 && !keypress.getKey('a') && !keypress.getKey('d'))
				{
					xAccel = 0;
				}

			}
		} //New physics
		else
		{
			y = 0;
			yAccel = 0;
		}




		if (keypress.getKey('w'))
		{
			yAccel += 1.5;
		}

		if (keypress.getKey('a'))
		{
			xAccel -= 0.1;
		}

		if (keypress.getKey('s'))
		{
			y -= 0.5;
		}

		if (keypress.getKey('d'))
		{
			xAccel += 0.1;
		}


		y += (yAccel * dT * dilution);
		x += (xAccel * dT * dilution);
	}
	else
	{
		if (keypress.getKey('w'))
		{
			y += 0.05;
		}

		if (keypress.getKey('a'))
		{
			x -= 0.05;
		}

		if (keypress.getKey('s'))
		{
			y -= 0.05;
		}

		if (keypress.getKey('d'))
		{
			x += 0.05;
		}
	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//cout << action;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		keypress.setKey('w', false);

	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		keypress.setKey('s', false);

	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		keypress.setKey('a', false);

	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		keypress.setKey('d', false);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		keypress.setKey('w', true);

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		keypress.setKey('s', true);

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		keypress.setKey('a', true);

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		keypress.setKey('d', true);
}

GLFWwindow* startGraphics(int width, int height)
{
	//Initialize OpenGL
	if (!glfwInit())
	{
		cout << "\nGLFW Failed init.\n";
		throw new exception("GLFW Failed init.", 0);
	}
	glfwSetErrorCallback(error_callback);

	//Code for setting up the window

	static GLFWwindow* window = glfwCreateWindow(width, height, "THE PRESIDENTIAL PHYSICS GAME", NULL, NULL);
	if (!window)
	{
		throw new exception("Window did not open.", 1);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1);

	//Initialize GLExtensionWrangler
	if (glewInit() != GLEW_OK)
	{
		cout << "\nGLEW Failed init.\n";
		throw new exception("GLEW Failed init.", 2);
	}
	return window;
}

void genericDraw(float verticies[])
{

}

int main()
{

	int width=640, height=480;
	GLFWwindow* window = startGraphics(width,height);
	float ratio;

	 
	glGenBuffers(1, &buffer); //Turn that unsigned int into an actual buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //Set that buffer as the current buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //Tells OpenGL how to read the buffer, as a set of floats and in what dimensions
	//Vertex attributes: Starts at first index, has two indexes per vertex, the vertexes are floats, Do not normalize them, A single vertex is the size of two floats... idk
	glEnableVertexAttribArray(0);//Enables the options above

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT); //Clear screen
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		ratio = width / (float)height;
		frames++;

		float verticies[] = //Define a triangle around X,Y
		{
			(x-0.05),(y-0.05),
			(x),(y+0.05),
			(x+0.05),(y-0.05)
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), &verticies, GL_DYNAMIC_DRAW); //Send this info to the buffer
		glDrawArrays(GL_TRIANGLE_FAN, 0, 3); //Tell OpenGL to read the buffer as a series of points to make triangles
		physics();


		glDrawBuffer(buffer);
		glfwSetWindowOpacity(window, 1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	
	
	
	glfwTerminate();
	return 0;
}