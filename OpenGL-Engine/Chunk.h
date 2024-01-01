#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

#include "Cube.h"
#include "FastNoiseLite.h"
#include "Configuration.h"

const int treeId = -1;

class Chunk
{
public:
	Chunk(Camera camera, Cube* grass, Cube* water, Cube* wood, Cube* leaf, unsigned int seed, int offsetX, int offsetZ);

	void GenerateChunk();

	void RenderChunk(int uniformModel, int uniformProjection, int uniformView);
	void SaveMapInFile(const char* fileName);
	void LoadMapFromFile(const char* fileName);

	~Chunk();

	std::vector<std::vector<int>> chunk;
	std::vector<std::vector<int>> structures;

private:
	void InitializeChunk();
	bool isTopFaceShadow(int x, int z);
	bool isFrontShadow(int x, int z);
	bool isLeftShadow(int x, int z);
	bool isWater(int x, int y, int z);
	void drawSides(int x, int y, int z, Cube* cube);

	int offsetX;
	int offsetZ;
	unsigned int seed;
	std::string strSeed;

	Cube* grass;
	Cube* water;
	Cube* wood;
	Cube* leaf;

	std::vector<std::vector<std::pair<int, int>>> leafsPosition;

	std::hash<std::string> hashFunction;
	FastNoiseLite noiseGenerator;

	Camera camera;
};

