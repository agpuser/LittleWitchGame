/*
SIT153 Introduction to Programming
Assignment 2 - Arcade Game
Aaron Pethybridge
Student#: 217561644

Class: WitchGame
Class declaration
Controlling / Container class to setup and run game
This class does the following:
* Initialises game and Scene
* Load Initialising data for game sprites and related info
* Initialises sprites and game data
* Runs program and game loop
* Display title screen and handle screen input
* Execute game loop, handle input, update game and draw game state
*/
#pragma once
#include <vector>
#include "math_t.h"
#include "Geometry.h"
#include "WinCanvas.h"
#include "InputState.h"
#include "DerivedClasses.h"
#include "TextureManager.h"
#include "Scene.h"

#define VK_X 0x58 // Virtual key code defined for "X" key

using namespace std;

/*
 * Type used to store point and velocity data for ghost enemy sprites
 * when read from enemies.dat file
 */
struct enemy_data {
	POINT2f coords;
	VECTOR2f direction;
};

class WitchGame{
private:
	bool gamefinished = false; // The flag that the game loop uses to see if the game has finished
	bool programFinished = false; // flag to end program
	vector<enemy_data> enemiesData; // Store enemy input file data
	vector<POINT2f> mazeCoords; // Store maze block input file data
	vector<POINT2f> purpleGemsCoords; // Store purple gem input file data
	vector<POINT2f> greenGemsCoords; // Store green gem input file data
	vector<POINT2f> orangeGemsCoords; // Store orange gem input file data
	vector<POINT2f> scoreGems;

	// Create Scene to display game and sprites
	Scene m_world = {
		POINT2(0,0),		// Upper left corner coordinate
		POINT2(G_WINDOW_WIDTH, G_WINDOW_HEIGHT)		// lower right corner coordinate
	};
	int level; // game difficulty level: 0 - Apprentice, 1 - Adept, 2 - Master
	vector<std::shared_ptr<Gem>> m_scoreboard; // Stores sprites for displaying no. of gems player has
	std::shared_ptr<GameSprite> m_card; // Sprite used to display messages
	TextureManager m_tm; // Handles Texture loading, storing and processing
	PLAYER_PTR m_player; // Player sprite
			
public:
	static const unsigned int	G_WINDOW_WIDTH = 810;
	static const unsigned int	G_WINDOW_HEIGHT = 640;

	WitchGame();
	~WitchGame() {}

	// Member function prototypes
	vector<enemy_data> loadEnemies();
	vector<POINT2f> loadMaze();
	vector<POINT2f> loadGems(gem_type t);
	void drawMazeCoords();
	int getNumMazeBlocks();
	vector<POINT2f> getMazeCoords();
	vector<POINT2f> getGemsCoords(gem_type t);
	vector<enemy_data> getEnemiesData();

	void runSplashScreen(WinCanvas &w, InputState &i);
	void runGame(WinCanvas &w, InputState &i);
	void initMazeBlocks();
	void initDoor();
	void initPlayer(InputState &is);
	void initGems(gem_type t);
	void initEnemies();
	void initScoreboard();
	void initCard();

	void updateScoreboard();
	void updateCard();

	int getLevel();
	void setLevel(int l);
	vector<POINT2f> getScoreGems();
	vector<POINT2f> loadScoreGems();
};