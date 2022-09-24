#version 430 core

in vec4 vPosition;
in vec4 vColor;

out vec4 color;

void main()
{
    //This is a simple pass-through shader. It does nothing special.
    color = vColor;
    gl_Position = vPosition;
}