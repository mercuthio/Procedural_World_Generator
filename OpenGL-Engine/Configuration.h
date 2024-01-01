#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const int MAX_HEIGHT = 7;
static const int WATER_LAYER = 0;
static const int CHUNK_SIZE = 32;
static const int CHUNK_NUMBERS = 3;
static const int TREE_HEIGHT = 5;

//Trees
static const float FOV = 60.0f;
static const float DRAW_DISTANCE = 500.0f;
static const float CUBE_SIZE = 1.0f;
static const float TREE_SPAWN_RATE = 0.001f;

static const double HILLY = 0.15;								// defines how abrupt the map will be generated (>0) the lower the more abrupt

// Ambient light
const glm::vec3 COLOR_AMBIENT_LIGHT = glm::vec3(1.0f, 1.0f, 0.7f);
const GLfloat COLOR_AMBIENT_INTENSITY = 0.8f;

//Directional light
const glm::vec3 LIGHT_DIRECTION = glm::vec3(-1.0f, 0.0f, 0.0f);
const GLfloat LIGHT_INTENSITY = 1.0f;
