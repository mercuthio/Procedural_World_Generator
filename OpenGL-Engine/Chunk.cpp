#include "Chunk.h"
#include <random>
#include <iostream>

using namespace glm;
using namespace std;

Chunk::Chunk(Camera camera, Cube* grass, Cube* water, Cube* wood, Cube* leaf, unsigned int seed, int offsetX, int offsetZ)
{
	this->camera = camera;
	this->offsetX = offsetX;
	this->offsetZ = offsetZ;
	this->seed = seed;
	this->strSeed = to_string(seed);

	chunk.resize(CHUNK_SIZE, vector<int>(CHUNK_SIZE));
	structures.resize(CHUNK_SIZE, vector<int>(CHUNK_SIZE));
	InitializeChunk();

	this->grass = grass;
	this->water = water;
	this->wood = wood;
	this->leaf = leaf;

	leafsPosition = {{ {1,0},{0,1},{0,0},{-1,0},{0,-1} },
					 { {1,0},{0,1},{-1,0},{0,-1},{1,1},{-1,-1},{1,-1},{-1,1} },
					 { {2,0},{0,2},{-2,0},{0,-2},{2,2},{-2,2},{2,-2},{1,1},{-1,-1},{-1,1},{1,-1}, {2,-1},{2,1},{1,2},{-1,2}, {-2,-1},{-2,1},{1,-2},{-1,-2}}, 
					 { {2,0},{0,2},{-2,0},{0,-2},{2,2},{-2,2},{2,-2},{1,1},{-1,-1},{-1,1},{1,-1}, {2,-1},{2,1},{1,2},{-1,2}, {-2,-1},{-2,1},{1,-2},{-1,-2}, {1,0},{0,1},{-1,0},{0,-1} } };

	noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Value);
	noiseGenerator.SetSeed(seed);
}

void Chunk::InitializeChunk()
{
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			chunk[x][z] = 0;
			structures[x][z] = 0;
		}
	}
}

void Chunk::GenerateChunk()
{
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			float sampleX = static_cast<float>(x + offsetX) / HILLY;
			float sampleY = static_cast<float>(z + offsetZ) / HILLY;

			float perlinValue = noiseGenerator.GetNoise(sampleX, sampleY);

			size_t combinedValue = ((static_cast<size_t>(seed) << 32) | (reinterpret_cast<size_t&>(perlinValue))) * seed;
			float normalizedHash = static_cast<float>(combinedValue) / static_cast<float>(std::numeric_limits<size_t>::max());

			if (normalizedHash < TREE_SPAWN_RATE)
			{
				structures[x][z] = -1;
			}
			
			int heightValue = static_cast<int>((perlinValue + 1.0) * 0.5 * MAX_HEIGHT);
			chunk[x][z] = heightValue;
		}
	}
}

bool Chunk::isTopFaceShadow(int x, int z)
{
	// Sun is in the west
	return (z < chunk.size() - 1 && chunk[x][z + 1] > chunk[x][z]);
}

bool Chunk::isFrontShadow(int x, int z)
{
	// Sun is in the west
	return z > 0 && chunk[x][z - 1] < chunk[x][z];
}

bool Chunk::isLeftShadow(int x, int z)
{
	// Sun is in the west
	return z < chunk.size() - 1 && x > 0 && chunk[x - 1][z + 1] >= chunk[x][z];
}

void Chunk::RenderChunk(int uniformModel, int uniformProjection, int uniformView)
{
	mat4 view = camera.calculateViewMatrix();
	Cube* currentCube = nullptr;

	for (int x = 0; x < chunk.size(); x++)
	{
		float xPos = (x * CUBE_SIZE) + offsetX;

		for (int z = 0; z < chunk[x].size(); z++)
		{
			float zPos = (z * CUBE_SIZE) + offsetZ;

			// Draw under cubes
			for (int y = 0; y < chunk[x][z] + 1; y++)
			{
				currentCube = isWater(x, y, z) ? water : grass;

				currentCube->PrepareRender(view, uniformModel, uniformProjection, uniformView,
					xPos, (y * CUBE_SIZE), zPos);

				if (y == chunk[x][z])
				{
					 isTopFaceShadow(x, z) ? currentCube->RenderTopShadowFace() : currentCube->RenderTopFace();
				}

				drawSides(x, y, z, currentCube);
			}

			// Tree
			if (structures[x][z] == treeId && chunk[x][z] != WATER_LAYER)
			{
				// Log
				for (int i = 0; i < TREE_HEIGHT; i++)
				{
					currentCube->PrepareRender(view, uniformModel, uniformProjection, uniformView,
						xPos, (chunk[x][z] + 1 + i) * CUBE_SIZE, zPos);

					wood->RenderSidesTop();
				}

				//Leafs
				int i = 0;
				for (const auto& layer : leafsPosition)
				{
					for (const auto& par : layer)
					{
						currentCube->PrepareRender(view, uniformModel, uniformProjection, uniformView,
							((x + par.first) * CUBE_SIZE) + offsetX,
							(chunk[x][z] + 1 + TREE_HEIGHT - i) * CUBE_SIZE,
							((z + par.second) * CUBE_SIZE) + offsetZ);

						if (i == leafsPosition.size() - 1)
						{
							leaf->RenderAllFaces();
						}
						else
						{
							leaf->RenderSidesTop();
							continue;
						}

						// Tree shadows
						if ((par.first + x >= 0 && par.first + x < chunk.size() - 1) &&
							(par.second + z >= 0 && par.second + z < chunk.size() - 1))
						{
							currentCube = grass;
							if (chunk[par.first + x][par.second + z] == WATER_LAYER)
							{
								currentCube = water;
							}

							currentCube->PrepareRender(view, uniformModel, uniformProjection, uniformView,
								((x + par.first) * CUBE_SIZE) + offsetX,
								chunk[par.first + x][par.second + z] * CUBE_SIZE,
								((z + par.second) * CUBE_SIZE) + offsetZ);

							currentCube->RenderTopShadowFace();
						}
					}
					i++;
				}
			}
		}
	}
}

bool Chunk::isWater(int x, int y , int z)
{
	int value = chunk[x][z];

	if ((x > 0 && chunk[x - 1][z] == WATER_LAYER && value < chunk[x - 1][z]) ||				 
		(z > 0 && chunk[x][z - 1] == WATER_LAYER && value < chunk[x][z - 1]) ||
		(x < chunk.size() - 1 && chunk[x + 1][z] == WATER_LAYER && value < chunk[x + 1][z]) ||
		(z < chunk.size() - 1 && chunk[x][z + 1] == WATER_LAYER && value < chunk[x][z + 1]))
	{
		if (y == value)
		{
			chunk[x][z] = WATER_LAYER;									// Fill water
		}
	}

	bool isWater = (y == WATER_LAYER);								// Its the water layer
	isWater &= (y == value);										// Its the top layer

	return isWater;
}

void Chunk::drawSides(int x, int y, int z, Cube* cube)
{
	// Draw front face
	if ((z < chunk[0].size() - 1 && y > chunk[x][z + 1]) || (z == chunk[0].size() - 1))
	{
		cube->RenderFrontFace();
	}

	// Draw back face
	if (z > 0 && y > chunk[x][z - 1] || (z == 0))
	{
		if (isFrontShadow(x, z))
		{
			cube->RenderBackShadowFace();
		}
		else
		{
			cube->RenderBackFace();
		}
	}

	// Draw right face
	if (x < chunk.size() - 1 && y > chunk[x + 1][z] || (x == chunk[0].size() - 1))
	{
		cube->RenderRightFace();
	}

	// Draw left face
	if (x > 0 && y > chunk[x - 1][z] || (x == 0))
	{
		if (isLeftShadow(x, z))
		{
			cube->RenderLeftShadowFace();
		}
		else
		{
			cube->RenderLeftFace();
		}
	}
}

void Chunk::SaveMapInFile(const char* fileName)
{
	ofstream file(fileName);

	if (!file.is_open()) {
		printf("Failed to create file %s!.", fileName);
		return;
	}

	for (const vector<int>& x : chunk)
	{
		for (const int z : x)
		{
			file << z << " ";
		}
		file << "-\n";
	}

	file.close();
}

void Chunk::LoadMapFromFile(const char* fileName)
{
	chunk.clear();

	string content;
	ifstream fileStream(fileName, ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileName);
		return ;
	}

	string value = "";
	vector<int> row;
	while (!fileStream.eof())
	{
		fileStream >> value;

		if (value == "-")
		{
			chunk.push_back(row);
			row.clear();
			continue;
		}

		row.push_back(stoi(value));
	}

	fileStream.close();
}

Chunk::~Chunk() {}

