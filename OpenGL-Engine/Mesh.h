#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderFace(GLuint face, GLuint textureId);
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
};

