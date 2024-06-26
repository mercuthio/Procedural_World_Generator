#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Chunk.h"
#include "Texture.h"
#include "Light.h"
#include "ImGuiManager.h"

using namespace glm;

unsigned int seed = time(nullptr);

const float toRadians = 3.14159265f / 180.0f;

int firstRenders = 5;

mat4 projection;

Window mainWindow;
GLFWwindow* guiWindow;

ImGuiManager* gui;
std::vector<Shader> shaderList;

Camera camera;

Light light;

Texture dirtText;
Texture dirtShadowText;
Texture grassText;
Texture grassShadowText;
Texture stoneText;
Texture stoneShadowText;
Texture waterText;
Texture waterShadowText;
Texture woodText;
Texture leafText;

Cube* grass;
Cube* stone;
Cube* water;
Cube* wood;
Cube* leaf;

Chunk* chunk;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, 
	   uniformAmbientColour = 0, uniformAmbientIntensity = 0, 
	   uniformDirection = 0, uniformDiffuseIntensity = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat now = 0.0f;

GLfloat moveXChange = 0.0f;
GLfloat moveZChange = 0.0f;
GLfloat prevXpos = 0.0f;
GLfloat prevZpos = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

std::vector<std::pair<int, int>> chunksNorth;
std::vector<std::pair<int, int>> chunksNorthEast;
std::vector<std::pair<int, int>> chunksEast;
std::vector<std::pair<int, int>> chunksSouthEast;
std::vector<std::pair<int, int>> chunksSouth;
std::vector<std::pair<int, int>> chunksSouthWest;
std::vector<std::pair<int, int>> chunksWest;
std::vector<std::pair<int, int>> chunksNorthWest;

void Reshape(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
}

void CreateObjects() 
{
	dirtText = Texture("Textures/mineDirt.png");
	dirtText.LoadTexture();
	dirtShadowText = Texture("Textures/mineDirtShadow.png");
	dirtShadowText.LoadTexture();
	grassText = Texture("Textures/mineGrass.png");
	grassText.LoadTexture();
	grassShadowText = Texture("Textures/mineGrassShadow.png");
	grassShadowText.LoadTexture();

	stoneText = Texture("Textures/mineStone.png");
	stoneText.LoadTexture();
	stoneShadowText = Texture("Textures/mineStoneShadow.png");
	stoneShadowText.LoadTexture();

	waterText = Texture("Textures/mineWater.png");
	waterText.LoadTexture();
	waterShadowText = Texture("Textures/mineWaterShadow.png");
	waterShadowText.LoadTexture();

	woodText = Texture("Textures/mineWood.png");
	woodText.LoadTexture();

	leafText = Texture("Textures/mineLeaf.png");
	leafText.LoadTexture();

	grass = new Cube(projection, grassText.GetTextId(), dirtText.GetTextId(), grassShadowText.GetTextId(), dirtShadowText.GetTextId());
	stone = new Cube(projection, stoneText.GetTextId(), stoneText.GetTextId(), stoneShadowText.GetTextId(), stoneShadowText.GetTextId());
	water = new Cube(projection, waterText.GetTextId(), waterText.GetTextId(), waterShadowText.GetTextId(), waterShadowText.GetTextId());
	wood = new Cube(projection, woodText.GetTextId(), woodText.GetTextId());
	leaf = new Cube(projection, leafText.GetTextId(), leafText.GetTextId());

	light = Light(COLOR_AMBIENT_LIGHT, COLOR_AMBIENT_INTENSITY, LIGHT_DIRECTION, LIGHT_INTENSITY);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void PreCalculateChunks()
{
	chunksNorth.clear();
	chunksNorthEast.clear();
	chunksEast.clear();
	chunksSouthEast.clear();
	chunksSouth.clear();
	chunksSouthWest.clear();
	chunksWest.clear();
	chunksNorthWest.clear();

	for (int i = -CHUNK_NUMBERS; i <= CHUNK_NUMBERS; ++i)
	{
		for (int j = 0; j <= CHUNK_NUMBERS; ++j)
		{
			chunksNorth.push_back({ i, j });
		}
	}

	for (int i = 0; i <= CHUNK_NUMBERS; ++i)
	{
		for (int j = -3; j <= CHUNK_NUMBERS; ++j)
		{
			chunksNorthEast.push_back({ i, j });
		}
	}

	for (int i = -CHUNK_NUMBERS; i <= CHUNK_NUMBERS; ++i)
	{
		for (int j = 0; j <= CHUNK_NUMBERS; ++j)
		{
			chunksEast.push_back({ j, i });
		}
	}

	for (int i = -3; i <= CHUNK_NUMBERS; ++i)
	{
		for (int j = 0; j >= -CHUNK_NUMBERS; --j)
		{
			chunksSouthEast.push_back({ i, j });
		}
	}

	for (int i = -CHUNK_NUMBERS; i <= CHUNK_NUMBERS; ++i)
	{
		for (int j = 0; j >= -CHUNK_NUMBERS; --j)
		{
			chunksSouth.push_back({ i, j });
		}
	}

	for (int i = -CHUNK_NUMBERS; i <= 0; ++i)
	{
		for (int j = 3; j >= -CHUNK_NUMBERS; --j)
		{
			chunksSouthWest.push_back({ i, j });
		}
	}

	for (int i = -CHUNK_NUMBERS; i <= CHUNK_NUMBERS; ++i)
	{
		for (int j = -CHUNK_NUMBERS; j <= 0; ++j)
		{
			chunksWest.push_back({ j, i });
		}
	}

	for (int i = -CHUNK_NUMBERS; i <= 3; ++i)
	{
		for (int j = 0; j <= CHUNK_NUMBERS; ++j)
		{
			chunksNorthWest.push_back({ i, j });
		}
	}
}

std::vector<std::pair<int, int>> getChunks(Camera camera)
{
	switch (camera.GetOrientation())
	{
	case Orientation::NORTH:
		return chunksNorth;
	case Orientation::NORTHEAST:
		return chunksNorthEast;
	case Orientation::EAST:
		return chunksEast;
	case Orientation::SOUTHEAST:
		return chunksSouthEast;
	case Orientation::SOUTH:
		return chunksSouth;
	case Orientation::SOUTHWEST:
		return chunksSouthWest;
	case Orientation::WEST:
		return chunksWest;
	case Orientation::NORTHWEST:
		return chunksNorthWest;
	}
}

void CreateGUI()
{
	std::string strFps = "FPS: " + std::to_string(static_cast<int>(mainWindow.getFps(deltaTime)));
	std::string strSeed = "Seed: " + std::to_string(seed);

	gui->StartWindow(GUI_NAME, GUI_WIDTH, GUI_HEIGHT);

	gui->AddText(strSeed.c_str());
	gui->AddSameLine();
	gui->AddText(strFps.c_str());


	gui->AddSliderInt("Chunk Size", &CHUNK_SIZE, 1, 50);
	gui->AddSliderInt("Chunks", &CHUNK_NUMBERS, 1, 20);
	gui->AddSeparator();

	gui->AddSliderInt("Max Height", &MAX_HEIGHT, 1, 50);
	gui->AddSliderFloat("Flat", &FLAT, 0.01, 1);
	gui->AddSliderInt("Water Layer", &WATER_LAYER, 0, MAX_HEIGHT);
	gui->AddSeparator();

	gui->AddSliderFloat("Tree Rate", &TREE_SPAWN_RATE, 0, 0.03);
	gui->AddSliderInt("Tree Height", &TREE_HEIGHT, 1, 20);
	gui->AddSliderInt("Stone Grass Gap", &STONE_GRASS_GAP, 0, 5);

	gui->AddSeparator();
	gui->AddSliderFloat("Ambient Luminosity", &COLOR_AMBIENT_INTENSITY, 0, 10);

	ImVec4 aux;
	aux.x = COLOR_AMBIENT_LIGHT.x;
	aux.y = COLOR_AMBIENT_LIGHT.y;
	aux.z = COLOR_AMBIENT_LIGHT.z;

	gui->AddColorPicker("Ambient Color", "Color", aux);

	COLOR_AMBIENT_LIGHT.x = aux.x;
	COLOR_AMBIENT_LIGHT.y = aux.y;
	COLOR_AMBIENT_LIGHT.z = aux.z;

	gui->FinishWindow();
}

void RenderChunks()
{
	PreCalculateChunks();

	for (const auto& par : getChunks(camera))
	{
		float offsetX = par.second * CHUNK_SIZE + camera.GetXposition() - CHUNK_SIZE / 2;
		float offsetZ = par.first * CHUNK_SIZE + camera.GetZposition() - CHUNK_SIZE / 2;

		chunk = new Chunk(camera, grass, stone, water, wood, leaf, seed, offsetX, offsetZ);
		chunk->GenerateChunk();

		chunk->RenderChunk(uniformModel, uniformProjection, uniformView);
	}
}

void RenderWorld()
{
	// Clear the window
	glClearColor(0.53f, 0.8f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
	uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
	uniformDirection = shaderList[0].GetDirectionLocation();
	uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();

	light.UseLight(uniformAmbientIntensity, uniformAmbientColour,
		uniformDirection, uniformDiffuseIntensity);

	RenderChunks();
}

int main() 
{
	mainWindow = Window(WINDOW_WIDTH, WINDOW_HEIGHT);
	mainWindow.Initialise();
	mainWindow.SetResizeCallBack(Reshape);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	guiWindow = glfwCreateWindow(GUI_WIDTH, GUI_HEIGHT, GUI_NAME, NULL, NULL);

	projection = perspective(radians(FOV), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, DRAW_DISTANCE);
	camera = Camera(vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 50.0f, 0.5f);
	
	gui = new ImGuiManager();
	gui->Initialise(guiWindow);

	CreateObjects();
	CreateShaders();

	// Loop until window closed
	while (!mainWindow.getShouldClose() && !glfwWindowShouldClose(guiWindow))
	{
		light = Light(COLOR_AMBIENT_LIGHT, COLOR_AMBIENT_INTENSITY, LIGHT_DIRECTION, LIGHT_INTENSITY);

		now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		
		glfwMakeContextCurrent(guiWindow);
		CreateGUI();
		gui->RenderWindow();
		glfwMakeContextCurrent(mainWindow.getWindow());

		RenderWorld();


		glUseProgram(0);

		mainWindow.swapBuffers();
		glfwSwapBuffers(guiWindow);
	}

	return 0;
}