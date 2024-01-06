#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <GL/glew.h>

static const char* WINDOW_NAME = "Engine";
static const char* GUI_NAME = "Configuration";

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
static const int GUI_WIDTH = 300;
static const int GUI_HEIGHT = 275;
extern int MAX_HEIGHT;
extern int WATER_LAYER;
extern int CHUNK_SIZE;
extern int CHUNK_NUMBERS;
extern int TREE_HEIGHT;
extern int STONE_GRASS_GAP;

//Trees
static const float FOV = 60.0f;
static const float DRAW_DISTANCE = 500.0f;
static const float CUBE_SIZE = 1.0f;
extern float TREE_SPAWN_RATE;

extern float FLAT;								// defines how abrupt the map will be generated (>0) the lower the more abrupt

//Ambient light
extern glm::vec3 COLOR_AMBIENT_LIGHT;
extern GLfloat COLOR_AMBIENT_INTENSITY;

//Directional light
const glm::vec3 LIGHT_DIRECTION = glm::vec3(-1.0f, 0.0f, 0.0f);
const GLfloat LIGHT_INTENSITY = 1.0f;
