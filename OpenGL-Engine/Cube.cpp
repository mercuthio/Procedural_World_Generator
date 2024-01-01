#include "Cube.h"
#include "Configuration.h"
using namespace glm;

Cube::Cube() {}

Cube::Cube(glm::mat4 projection,  GLuint topTextureID, GLuint mainTextureID, GLuint shadowTextureID, GLuint mainShadowTextureID)
{
	this->projection = projection;

	unsigned int numVertices = sizeof(cubeVertices) / sizeof(GLfloat);
	unsigned int numIndices = sizeof(cubeIndices) / sizeof(GLfloat);

	calcAverageNormals(8, 5);

	this->mesh = new Mesh();
	this->mesh->CreateMesh(cubeVertices, cubeIndices, numVertices, numIndices);

	this->topTextureID = topTextureID;
	this->topShadowTextureID = shadowTextureID;
	this->mainTextureID = mainTextureID;
	this->mainShadowTextureID = mainShadowTextureID;
}

Cube::Cube(const glm::mat4 projection, GLuint mainTextureID, GLuint mainShadowTextureID)
{
	this->projection = projection;

	unsigned int numVertices = sizeof(cubeVertices) / sizeof(GLfloat);
	unsigned int numIndices = sizeof(cubeIndices) / sizeof(GLfloat);

	calcAverageNormals(8, 5);

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

void Cube::PrepareRender(mat4 view, int uniformModel, int uniformProjection, int uniformView, float x, float y, float z)
{
	mat4 model(1.0f);
	model = translate(model, vec3(x, y, z));
	model = scale(model, vec3(CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(view));
}

void Cube::calcAverageNormals(unsigned int vLength, unsigned int normalOffset)
{
	unsigned int numVertices = sizeof(cubeVertices) / sizeof(GLfloat);
	unsigned int numIndices = sizeof(cubeIndices) / sizeof(GLfloat);

	for (size_t i = 0; i < numIndices; i += 3)
	{
		unsigned int in0 = cubeIndices[i] * vLength;
		unsigned int in1 = cubeIndices[i + 1] * vLength;
		unsigned int in2 = cubeIndices[i + 2] * vLength;
		glm::vec3 v1(cubeVertices[in1] - cubeVertices[in0], cubeVertices[in1 + 1] - cubeVertices[in0 + 1], cubeVertices[in1 + 2] - cubeVertices[in0 + 2]);
		glm::vec3 v2(cubeVertices[in2] - cubeVertices[in0], cubeVertices[in2 + 1] - cubeVertices[in0 + 1], cubeVertices[in2 + 2] - cubeVertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		cubeVertices[in0] += normal.x; cubeVertices[in0 + 1] += normal.y; cubeVertices[in0 + 2] += normal.z;
		cubeVertices[in1] += normal.x; cubeVertices[in1 + 1] += normal.y; cubeVertices[in1 + 2] += normal.z;
		cubeVertices[in2] += normal.x; cubeVertices[in2 + 1] += normal.y; cubeVertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < numVertices / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(cubeVertices[nOffset], cubeVertices[nOffset + 1], cubeVertices[nOffset + 2]);
		vec = glm::normalize(vec);
		cubeVertices[nOffset] = vec.x; cubeVertices[nOffset + 1] = vec.y; cubeVertices[nOffset + 2] = vec.z;
	}
}

Cube::~Cube()
{
}

