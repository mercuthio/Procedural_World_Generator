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
	//	x		y		z		u		v
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		//0
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		//1
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f,		//2
		1.0f, 1.0f, 0.0f,		1.0f, 1.0f,		//3

		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		//4
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		//5
		-1.0f, 1.0f, -1.0f,		1.0f, 1.0f,		//6
		1.0f, 1.0f, -1.0f,		0.0f, 1.0f,		//7

		// Top face
		-1.0f, 1.0f, 0.0f,		0.0f, 0.0f,		//8
		-1.0f, 1.0f, -1.0f,		0.0f, 1.0f,		//9
		1.0f, 1.0f, 0.0f,		1.0f, 0.0f,		//10
		1.0f, 1.0f, -1.0f,		1.0f, 1.0f,		//11

		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f,		//12
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		//13
		1.0f, -1.0f, 0.0f,		1.0f, 1.0f,		//14
		1.0f, -1.0f, -1.0f,		1.0f, 0.0f,		//15
};

static unsigned int cubeIndices[] = {
	0, 1, 2,		//front
	1, 2, 3,		//front

	4, 5, 6,		//back
	5, 6, 7,		//back

	4, 0, 6,		//left
	6, 2, 0,		//left

	5, 1, 3,		//right
	7, 3, 5,		//right

	8, 9, 10,		//top
	9, 11, 10,		//top

	12, 13, 14,		//bottom
	13, 15, 14,		//bottom
};

class Cube
{
public:
	Cube();
	Cube(glm::mat4 projection, Camera* camera, GLuint topTextureID, GLuint mainTextureID, GLuint shadowTextureID, GLuint mainShadowTextureID);
	Cube(glm::mat4 projection, Camera* camera, GLuint mainTextureID, GLuint mainShadowTextureID);

	void PrepareRender(int uniformModel, int uniformProjection, int uniformView, float x, float y, float z);
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
	glm::mat4 projection;
	Camera* camera;
};
