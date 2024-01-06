#pragma once

#include "Mesh.h"
#include "Camera.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

const int FRONT_FACE = 0;
const int BACK_FACE = 6;
const int LEFT_FACE = 12;
const int RIGHT_FACE = 18;
const int TOP_FACE = 24;
const int BOTTOM_FACE = 30;

static GLfloat cubeVertices[] = {
	//	x		y		z		u		v		nx	  ny    nz
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0,		0.0f, 0.0f, 0.0f,	//0
		1.0f, -1.0f, 0.0f,		1.0f, 0.0,		0.0f, 0.0f, 0.0f,	//1
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0,		0.0f, 0.0f, 0.0f,	//2
		1.0f, 1.0f, 0.0f,		1.0f, 1.0,		0.0f, 0.0f, 0.0f,	//3

		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//4
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//5
		-1.0f, 1.0f, -1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,	//6
		1.0f, 1.0f, -1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	//7

		// Top face
		-1.0f, 1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//8
		-1.0f, 1.0f, -1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	//9
		1.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//10
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,	//11

		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	//12
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//13
		1.0f, -1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,	//14
		1.0f, -1.0f, -1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//15
};

static unsigned int cubeIndices[] = {
    0, 2, 1,		// front
    1, 2, 3,		// front

	5, 7, 4,		//back
	4, 7, 6,		//back

	4, 6, 0,		//left
	0, 6, 2,		//left

	1, 3, 5,		//right
	5, 3, 7,		//right

	11, 10, 9,		//top
	9, 10, 8,		//top

	13, 12, 15,		//bottom
	15, 12, 14,		//bottom
};

class Cube
{
public:
	Cube();
	Cube(glm::mat4 projection, GLuint topTextureID, GLuint mainTextureID, GLuint shadowTextureID, GLuint mainShadowTextureID);
	Cube(glm::mat4 projection, GLuint mainTextureID, GLuint mainShadowTextureID);

	void PrepareRender(glm::mat4 view, int uniformModel, int uniformProjection, int uniformView, float x, float y, float z);
	void RenderAllFaces();
	void RenderSidesTop();
	void RenderTopFace();
	void RenderTopShadowFace();
	void RenderLeftFace();
	void RenderLeftShadowFace();
	void RenderRightFace();
	void RenderBackShadowFace();
	void RenderFrontFace();
	void RenderBackFace();

	~Cube();

	Mesh* mesh;
	GLuint topTextureID;
	GLuint topShadowTextureID;
	GLuint mainTextureID;
	GLuint mainShadowTextureID;

private:
	void calcAverageNormals(unsigned int vLength, unsigned int normalOffset);

	glm::mat4 projection;
};

