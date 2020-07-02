#pragma once
#include "headers.h"
using namespace std;

class VertexBuffer
{
private:
	Point* vertexes;
	int vertexCount;
	unsigned int buffer;

	float* vertexTranslator();

public:
	VertexBuffer();
	VertexBuffer(int vertexCount);
	VertexBuffer(int vertexCount, Point* vertexes);
	~VertexBuffer();

	Point getVertex(int index);
	void setVertex(int index, Point newValue);

	void updateVertexCount();
	void updateVertexCount(int newCount);

	void generateBuffer(GLenum Usage);
	unsigned int getBuffer();
	void setBuffer(unsigned int newBuffer);

	void Bind();
	void Unbind();
};

