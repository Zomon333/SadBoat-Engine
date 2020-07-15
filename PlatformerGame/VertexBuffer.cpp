#pragma once
#include "headers.h"
#include "VertexBuffer.h"
using namespace std;

float* VertexBuffer::vertexTranslator()
{
	float* toReturn = new float[vertexCount * 2];
	int i = 0;
	while (i < (vertexCount * 2))
	{
		if ((i % 2) == 0)
		{
			toReturn[i] = getVertex((i / 2)).x;
		}
		else
		{
			toReturn[i] = getVertex((i / 2)).y;
		}
		i++;
	}
	return toReturn;
}

VertexBuffer::VertexBuffer()
{

}

VertexBuffer::~VertexBuffer()
{
	delete[] vertexes;
	GLCall(glDeleteBuffers(1, &buffer));
}

void VertexBuffer::translate(float x, float y)
{
	int i = 0;
	Point workingPoint;
	while (i < vertexCount)
	{
		workingPoint = getVertex(i);
		workingPoint.translate(x, y);
		setVertex(i, workingPoint);
		i++;
	}
}

Point VertexBuffer::getVertex(int index)
{
	return vertexes[index];
}

void VertexBuffer::setVertex(int index, Point newValue)
{
	vertexes[index] = newValue;
}

void VertexBuffer::updateVertexCount()
{
	int i = 0;
	Point* toTest;
	toTest = &vertexes[i];
	while (toTest != NULL)
	{
		toTest = &vertexes[i];
		i++;
	}
	if (toTest == NULL)
	{
		this->vertexCount = i;
	}
}

void VertexBuffer::updateVertexCount(int newCount)
{
	this->vertexCount = newCount;
}

void VertexBuffer::generateBuffer(GLenum Usage)
{
	GLCall(glGenBuffers(1, &buffer));
	this->Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, ((this->vertexCount * 2)*sizeof(float)), this->vertexTranslator(), Usage));
}

unsigned int VertexBuffer::getBuffer()
{
	return this->buffer;
}

void VertexBuffer::setBuffer(unsigned int newBuffer)
{
	this->buffer = newBuffer;
}


void VertexBuffer::Bind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
}

void VertexBuffer::Unbind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
