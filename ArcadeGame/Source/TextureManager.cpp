/*
SIT153 Introduction to Programming
Assignment 2 - Arcade Game
Aaron Pethybridge
Student#: 217561644

Class : TextureManager
Function definitions
*/
#include "TextureManager.h"
#include <fstream>

// Constructor
TextureManager::TextureManager()
{
	texturesLoaded = loadTextures(); // load textures and data from file
}

/*
 * Loads texture label, file resource & transparency data
 */
bool TextureManager::loadTextures()
{
	bool result = false;
	string texturePath, textureLabel;
	int r, g, b;

	ifstream texturesFile;
	texturesFile.open("resource//textures.dat");
	if (!texturesFile.is_open())
		cout << "Textures file error.";
	else
	{
		while (!texturesFile.eof())
		{
			texturesFile >> textureLabel;
			texturesFile >> texturePath;
			texturesFile >> r;
			texturesFile >> g;
			texturesFile >> b;
			m_textures.push_back(std::make_shared<WinTexture>(texturePath, RGB(r, g, b)));
			m_labels.push_back(textureLabel);
		}
		texturesFile.close();
	}
	return result;
}

/*
 * Return texture based on label name
 */
TEXTURE_PTR TextureManager::getTexture(string label)
{
	int texturePos = 0;
	texturePos = getLabelIndex(label); // obtain "label" index
	return m_textures[texturePos];
}

/*
 * Return index of requested label from label storage 
 */
int TextureManager::getLabelIndex(string label)
{
	int pos = -1;

	for (unsigned int i = 0; i < m_labels.size(); i++)
	{
		if (m_labels[i] == label)
			pos = i;
	}
	return pos;
}