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