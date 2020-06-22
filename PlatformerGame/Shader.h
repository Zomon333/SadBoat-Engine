#pragma once
#include "headers.h"
using namespace std;

class Shader
{
private:
	string vs;
	string fs;
	unsigned int shaderProgram;

public:
	Shader();
	Shader(string filename);
	Shader(string vs, string fs);

	void updateVec4Uniform(float r, float g, float b, float a, string name);
	unsigned int loadShader(string fileName);
	unsigned int compileShader(unsigned int type, const string& source);

	unsigned int createShader(string& vertexShader, string& fragmentShader);
	unsigned int createShader();
};