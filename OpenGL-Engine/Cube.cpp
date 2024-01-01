#include "Cube.h"
#include "Configuration.h"
using namespace glm;

Cube::Cube() {}

Cube::Cube(glm::mat4 projection, Camera* camera, GLuint topTextureID, GLuint mainTextureID, GLuint shadowTextureID, GLuint mainShadowTextureID)
{
	this->camera = camera;
	this->projection = projection;

	unsigned int numVertices = sizeof(cubeVertices) / sizeof(GLfloat);
	unsigned int numIndices = sizeof(cubeIndices) / sizeof(GLfloat);

	this->mesh = new Mesh();
	this->mesh->CreateMesh(cubeVertices, cubeIndices, numVertices, numIndices);

	this->topTextureID = topTextureID;
	this->topShadowTextureID = shadowTextureID;
	this->mainTextureID = mainTextureID;
	this->mainShadowTextureID = mainShadowTextureID;
}

Cube::Cube(const glm::mat4 projection, Camera* camera, GLuint mainTextureID, GLuint mainShadowTextureID)
{
	this->camera = camera;
	this->projection = projection;

	unsigned int numVertices = sizeof(cubeVertices) / sizeof(GLfloat);
	unsigned int numIndices = sizeof(cubeIndices) / sizeof(GLfloat);

	this->mesh = new Mesh();
	this->mesh->CreateMesh(cubeVertices, cubeIndices, numVertices, numIndices);

	this->topTextureID = mainTextureID;
	this->topShadowTextureID = mainShadowTextureID;
	this->mainTextureID = mainTextureID;
	this->mainShadowTextureID = mainShadowTextureID;
}

void Cube::RenderAllFaces()
{
	mesh->RenderFace(TOP_FACE, topTextureID);
	mesh->RenderFace(LEFT_FACE, mainTextureID);
	mesh->RenderFace(RIGHT_FACE, mainTextureID);
	mesh->RenderFace(FRONT_FACE, mainTextureID);
	mesh->RenderFace(BACK_FACE, mainTextureID);
	mesh->RenderFace(BOTTOM_FACE, topTextureID);
}

void Cube::RenderSidesTop()
{
	mesh->RenderFace(TOP_FACE, topTextureID);
	mesh->RenderFace(LEFT_FACE, mainTextureID);
	mesh->RenderFace(RIGHT_FACE, mainTextureID);
	mesh->RenderFace(FRONT_FACE, mainTextureID);
	mesh->RenderFace(BACK_FACE, mainTextureID);
}

void Cube::RenderTopFace()
{
	mesh->RenderFace(TOP_FACE, topTextureID);
}

void Cube::RenderTopShadowFace()
{
	mesh->RenderFace(TOP_FACE, topShadowTextureID);
}

void Cube::RenderLeftFace()
{
	mesh->RenderFace(LEFT_FACE, mainTextureID);
}

void Cube::RenderLeftShadowFace()
{
	mesh->RenderFace(LEFT_FACE, mainShadowTextureID);
}

void Cube::RenderRightFace()
{
	mesh->RenderFace(RIGHT_FACE, mainTextureID);
}

void Cube::RenderFrontFace()
{
	mesh->RenderFace(FRONT_FACE, mainTextureID);
}

void Cube::RenderBackShadowFace()
{
	mesh->RenderFace(BACK_FACE, mainShadowTextureID);
}

void Cube::RenderBackFace()
{
	mesh->RenderFace(BACK_FACE, mainTextureID);
}

void Cube::PrepareRender(int uniformModel, int uniformProjection, int uniformView, float x, float y, float z)
{
	mat4 model(1.0f);
	model = translate(model, vec3(x, y, z));
	model = scale(model, vec3(CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(camera->calculateViewMatrix()));
}

Cube::~Cube()
{
}
