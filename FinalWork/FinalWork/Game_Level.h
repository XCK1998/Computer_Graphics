#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Game_Object.h"
#include "Sprite_Renderer.h"
#include "Resource_Manager.h"


/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
	// Level state
	std::vector<GameObject> Bricks;
	// Constructor
	GameLevel() { }
	// Loads level from file
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	// Render level
	void Draw(SpriteRenderer &renderer);
	// Check if the level is completed (all non-solid tiles are destroyed)
	GLboolean IsCompleted();
private:
	// Initialize level from tile data
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};
