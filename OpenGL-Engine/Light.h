#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 color, GLfloat aIntensity, glm::vec3 dir, GLfloat dIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		          GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~Light();

private:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

