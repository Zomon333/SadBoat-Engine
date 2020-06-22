#VERTEX
#version 330 core
layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}
#END
#FRAGMENT
#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 uColor; 
//Uniforms are a method of pulling a C++ variable into an OpenGL Shader without recompiling the shader every single time. It's a useful alternative to hard coding.

void main()
{
	color = uColor;
}
#END

