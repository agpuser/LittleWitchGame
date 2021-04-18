/*
SIT153 Introduction to Programming
Assignment 2 - Arcade Game
Aaron Pethybridge
Student#: 217561644

Class: TextureManager
Handles loading of texture files, storing texture data and
accessor methods to retrieve textures.
*/
#pragma once
#include "WinTexture.h"
#include <vector>
#include <string>
#include "Geometry.h"

using namespace std;

class TextureManager {
private:
	vector<string> m_labels; // Collection of labels used to refer to a particular texture within code
	vector<TEXTURE_PTR> m_textures; // Collection of loaded textures
	bool texturesLoaded; // Flag to record when textures are loaded
public:
	// Member functions
	TextureManager();
	~TextureManager() {}
	bool loadTextures();
	TEXTURE_PTR getTexture(string label);
	int getLabelIndex(string label);
};