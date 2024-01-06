#pragma once

#include "stdio.h"

#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	template <typename Func>
	void SetResizeCallBack(Func callback) { glfwSetWindowSizeCallback(mainWindow, callback); }
	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }
	GLFWwindow* getWindow() { return mainWindow; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	double getFps(GLfloat deltaTime);
	
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	void UpdateFps(GLfloat deltaTime);

	GLFWwindow* mainWindow;

	GLint numFrames;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	double fps;

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat startTime;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};