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