#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();

	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation() { return uniformProjection;  }
	GLuint GetModelLocation() { return uniformModel; }
	GLuint GetViewLocation() { return uniformView; }
	GLuint GetAmbientColourLocation() { return uniformAmbientColour; }
	GLuint GetAmbientIntensityLocation() { return uniformAmbientIntensity; }
	GLuint GetDiffuseIntensityLocation() { return uniformDiffuseIntensity; }
	GLuint GetDirectionLocation() { return uniformDirection; }

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

