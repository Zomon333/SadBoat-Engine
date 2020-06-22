#pragma once
#include "headers.h"
using namespace std;

Shader::Shader()
{
	Shader("default.shader");
}

Shader::Shader(string filename)
{
	shaderProgram = loadShader(filename);
}

Shader::Shader(string vs, string fs)
{
	shaderProgram = createShader(vs, fs);
}

void Shader::updateVec4Uniform(float r, float g, float b, float a, string name)
{
	int ColorLocation = glGetUniformLocation(shaderProgram, name.c_str());
	if(ColorLocation!=-1)
	glUniform4f(ColorLocation, r, g, b, a);	
}

inline unsigned int Shader::loadShader(string fileName)
{
	fstream file;
	file.open(fileName, 1);
	string workingMemory = "";
	bool vertex = false, fragment = false;
	if (file.is_open())
		while (file.get() != EOF)
		{
			file.unget();
			workingMemory += file.get();

			if (workingMemory == "\n" && !vertex && !fragment)
			{
				workingMemory = "";
			}
			if (workingMemory == "#END")
			{
				vertex = false;
				fragment = false;
				workingMemory = "";
				cout << "#END\n";
			}
			else
				if (workingMemory == "#VERTEX\n")
				{
					vertex = true;
					fragment = false;
					workingMemory = "";
					cout << "#VERTEX\n";
				}
				else
					if (workingMemory == "#FRAGMENT\n")
					{
						fragment = true;
						vertex = false;
						workingMemory = "";
						cout << "#FRAGMENT\n";
					}
					else
						if (fragment)
						{
							file.unget();
							if (file.get() == '\n')
							{
								this->fs += workingMemory;
								workingMemory = "";
							}

						}
						else
							if (vertex)
							{
								file.unget();
								if (file.get() == '\n')
								{
									this->vs += workingMemory;
									workingMemory = "";
								}
							}

		}
	else
	{
		cout << "File did not open.";
	}

	unsigned int shader = createShader(this->vs, this->fs);
	return shader;
}

inline unsigned int Shader::compileShader(unsigned int type, const string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); // This may have an issue. If so change char to char*
		glGetShaderInfoLog(id, length, &length, message);
		cout << "Failed to compile: " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << endl << message << endl;
		glDeleteShader(id);
		return 0;
	}


	return id;
}

inline unsigned int Shader::createShader(string& vertexShader, string& fragmentShader)
{
	this->vs = vertexShader;
	this->fs = fragmentShader;
	return createShader();
}

inline unsigned int Shader::createShader()
{
	unsigned int program = glCreateProgram();
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	vs = compileShader(GL_VERTEX_SHADER, this->vs);
	fs = compileShader(GL_FRAGMENT_SHADER, this->fs);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}