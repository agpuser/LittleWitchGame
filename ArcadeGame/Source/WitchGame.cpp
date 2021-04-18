/*
SIT153 Introduction to Programming
Assignment 2 - Arcade Game
Aaron Pethybridge
Student#: 217561644

Class: WitchGame
Member function definitions
*/
#include <fstream>
#include <string>
#include "WitchGame.h"

// Constructor
WitchGame::WitchGame()
{
	level = 1; // set level default to 1
	// load and store all sprite related data
	mazeCoords = loadMaze();
	enemiesData = loadEnemies();
	purpleGemsCoords = loadGems(PURPLE);
	greenGemsCoords = loadGems(GREEN);
	orangeGemsCoords = loadGems(ORANGE);
	scoreGems = loadScoreGems();
	//drawMazeCoords();
}

/*
 * Loads co-ordinate data from file for blocks that constitute maze sprites
 */
vector<POINT2f> WitchGame::loadMaze()
{
	vector<POINT2f> coords;
	POINT2f temp;

	ifstream mazeFile;
	mazeFile.open("resource//maze.dat");
	if (!mazeFile.is_open())
		cout << "Maze file error.";
	else
	{
		while (!mazeFile.eof())
		{
			mazeFile >> temp.x;
			mazeFile >> temp.y;
			coords.push_back(temp);
		}
		mazeFile.close();
	}
	return coords;
}

/*
* Loads co-ordinate data from file for gem sprites
*/
vector<POINT2f> WitchGame::loadGems(gem_type t)
{
	vector<POINT2f> result;
	POINT2f temp;
	string fname;

	ifstream gemsFile;
	switch (t)
	{
		case PURPLE:
		{
			fname = "resource//purple";
			break;
		}
		case GREEN:
		{
			fname = "resource//green";
			break;
		}
		default:
		{
			fname = "resource//orange";
			break;
		}
	}
	gemsFile.open(fname + "_gems.dat");
	if (!gemsFile.is_open())
		cout << "Gems file error.";
	else 
	{
		while (!gemsFile.eof())
		{
			gemsFile >> temp.x;
			gemsFile >> temp.y;
			result.push_back(temp);
		}
		gemsFile.close();
	}
	
	return result;
}

/*
* Loads co-ordinate and velccity data from file for ghost enemy sprites
*/
vector<enemy_data> WitchGame::loadEnemies()
{
	vector<enemy_data> data;
	enemy_data temp;

	ifstream enemiesFile;
	enemiesFile.open("resource//enemies.dat");
	if (!enemiesFile.is_open())
		cout << "Enemies file error.";
	else 
	{
		while (!enemiesFile.eof())
		{
			enemiesFile >> temp.coords.x;
			enemiesFile >> temp.coords.y;
			enemiesFile >> temp.direction.x;
			enemiesFile >> temp.direction.y;
			data.push_back(temp);
		}
		enemiesFile.close();
	}
	return data;
}

/*
* Outputs loaded maze co-ordinates to console
*/
void WitchGame::drawMazeCoords()
{
	for (unsigned int i = 0; i < mazeCoords.size(); i++)
	{
		cout << "Maze coord " << i << ": x-" <<
			mazeCoords[i].x << " y-" << mazeCoords[i].y
			<< endl;
	}
}

/*
 * Returns number of maze blocks
 */
int WitchGame::getNumMazeBlocks()
{
	return mazeCoords.size();
}

/*
* Returns all maze block co-ordinates data
*/
vector<POINT2f> WitchGame::getMazeCoords()
{
	return mazeCoords;
}

/*
* Returns all enemies co-ordinates & velocity data
*/
vector<enemy_data> WitchGame::getEnemiesData()
{
	return enemiesData;
}

/*
 * Display splash title screem and process input
 */
void WitchGame::runSplashScreen(WinCanvas &w, InputState &i)
{
	bool splashDone = false; // flag to set whnn title screen input has been handled

	// Display title screen texture
	w.Clear(255, 255, 255);
	w.DrawTexture(*(m_tm.getTexture("SPLASH_SCREEN_TEXTURE")), POINT2f(0.0, 0.0));
	w.Display();

	// process title screen input
	while (!splashDone)
	{
		if (i.isActive(VK_F1))
		{
			setLevel(1);
			splashDone = true;
		}

		if (i.isActive(VK_F2))
		{
			setLevel(2);
			splashDone = true;
		}

		if (i.isActive(VK_F3))
		{
			setLevel(3);
			splashDone = true;
		}

		if (i.isActive(VK_X))		// Check to see if the Game Loop should terminated
		{
			splashDone = true;	// Break out of this title screen loop
			programFinished = true;
			w.Terminate();			// Force a quite message to be passed to the 
									// message handler, which will close that thread
			continue;
		}
	}
}

/*
 *  Initialise maze block sprites and position 
 */
void WitchGame::initMazeBlocks()
{
	vector<POINT2f> maze;
	maze = getMazeCoords();

	// Initialise maze background
	GameSprite bg = { m_tm.getTexture("BG_TEXTURE"), POINT2f(0, 0) };
	std::shared_ptr<GameSprite> pBg;
	pBg = std::make_shared<GameSprite>(bg);
	pBg->SetPosition(POINT2f(0.0, 0.0)); // get values from stored data
	m_world.AddSprite(pBg);

	// Initialise maze blocks
	for (unsigned int i = 0; i < maze.size(); i++)
	{
		GameSprite gs = { m_tm.getTexture("MAZE_TEXTURE"), POINT2f(0, 0) };
		std::shared_ptr<GameSprite> pgs;
		pgs = std::make_shared<GameSprite>(gs);
		pgs->SetPosition(POINT2f(maze[i].x, maze[i].y)); // get values from stored data
		m_world.AddSprite(pgs);
		m_world.AddMazeBlock(pgs);
	}
}

/*
*  Initialise door sprite and position
*/
void WitchGame::initDoor()
{
	GameSprite door = { m_tm.getTexture("DOOR_TEXTURE"), POINT2f(0, 0) };
	std::shared_ptr<GameSprite> pdoor;
	pdoor = std::make_shared<GameSprite>(door);
	pdoor->SetPosition(POINT2f(0.0, 200.0));
	m_world.AddSprite(pdoor);
	m_world.AddDoor(pdoor);
}

/*
*  Initialise player sprite and position
*/
void WitchGame::initPlayer(InputState &is)
{
	// Initialise player sprite
	PlayerCharacter MyPlayer = { m_tm.getTexture("PLAYER_TEXTURE"), POINT2f(100,100) };
	std::shared_ptr<PlayerCharacter> pPlayer = std::make_shared<PlayerCharacter>(MyPlayer);
	// Initialise its state
	pPlayer->setGems(0);
	pPlayer->SetPosition(POINT2f(500, 500));
	pPlayer->SetLeftTexture(m_tm.getTexture("PLAYER_TEXTURE"));
	pPlayer->SetRightTexture(m_tm.getTexture("PLAYER_ALT_TEXTURE"));
	pPlayer->setGhostTexture(m_tm.getTexture("PLAYER_GHOST_TEXTURE"));
	pPlayer->setCuptTexture(m_tm.getTexture("CUP_TEXTURE"));
	pPlayer->Attach(&is);
	// Add it to our scene
	m_world.AddSprite(pPlayer);
	m_world.AddControlled(pPlayer);
	m_world.AddPlayer(pPlayer);
	m_player = pPlayer;
}

/*
*  Initialise gem sprites and position data
*/
void WitchGame::initGems(gem_type t)
{
	string label;
	int size = 0;
	vector<POINT2f> gems = getGemsCoords(t);
	int Level = getLevel();

	switch (t) // Branch based on gem colour
	{
	case PURPLE:
		label = "PURPLE";
		size = gems.size();
		break;
	case GREEN:
		label = "GREEN";
		if (Level == 3) // if Master (3) level chosen, don't draw green gem
			size = 0; 
		else // otherwise draw one for level 1 and 2
			size = 1;
		break;
	case ORANGE:
		label = "ORANGE";
		if (Level > 1) // Don't draw orange gem unless level is Apprentice (1)
			size = 0;
		else
			size = gems.size(); // otherwise draw all orange gem(s)
		break;
	}

	// Initialise gem sprites
	for (int i = 0; i < size; i++)
	{
		Gem gem = { t, m_tm.getTexture(label + "_GEM_TEXTURE"), POINT2f(0, 0) };
		std::shared_ptr<Gem> pGem;
		pGem = std::make_shared<Gem>(gem);
		pGem->SetPosition(POINT2f(gems[i].x, gems[i].y));
		m_world.AddSprite(pGem);
		m_world.AddGem(pGem);
	}
}

/*
 * Initialise ghost enemy sprites and position data
 */
void WitchGame::initEnemies()
{
	vector<enemy_data> enemies;
	enemies = getEnemiesData();
	int level = getLevel();
	for (int i = 0; i < level; i++)
	{
		EnemyCharacter enemy = { m_tm.getTexture("ENEMY_TEXTURE"), VECTOR2f(0.0, 1.0) };
		std::shared_ptr<EnemyCharacter> pEnemy = std::make_shared<EnemyCharacter>(enemy);
		pEnemy->SetVelocity(enemies[i].direction);
		// Initialise its state
		pEnemy->SetPosition(enemies[i].coords);
		// Add it to our scene
		m_world.AddSprite(pEnemy);
		m_world.AddDynamic(pEnemy);
		m_world.AddEnemy(pEnemy);
	}
}

/*
 * Initialise scoreboard gem sprites & position data
 */
void WitchGame::initScoreboard()
{
	vector<POINT2f> scoreGemsCoords = getScoreGems();
	for (unsigned int i = 0; i <= scoreGemsCoords.size() - 1; i++)
	{
		Gem gem = { PURPLE, m_tm.getTexture("CLEAR_GEM_TEXTURE"), POINT2f(0, 0) };
		std::shared_ptr<Gem> pGem;
		pGem = std::make_shared<Gem>(gem);
		pGem->SetPosition(POINT2f(scoreGemsCoords[i].x, scoreGemsCoords[i].y));
		m_world.AddSprite(pGem);
		m_scoreboard.push_back(pGem);
	}
}

/*
* Initialise message card sprites & position data
*/
void WitchGame::initCard()
{
	GameSprite card = { m_tm.getTexture("MSG_GAME_TEXTURE"), POINT2f(600.0, 400.0) };
	std::shared_ptr<GameSprite> pCard;
	pCard = std::make_shared<GameSprite>(card);
	pCard->SetPosition(POINT2f(600.0, 400.0));
	m_world.AddSprite(pCard);
	m_card = pCard;
}

/*
 * Set scoreboard gem sprites based on how many gems player has
 */
void WitchGame::updateScoreboard()
{
	int tscore = m_player->getGems();
	for (int i = 0; i <= (int)m_scoreboard.size() - 1; i++)
	{
		if (tscore <= i) {
			m_scoreboard[i]->SetTexture(m_tm.getTexture("CLEAR_GEM_TEXTURE"));
			//cout << "C" << endl;
		}
		else {
			m_scoreboard[i]->SetTexture(m_tm.getTexture("PURPLE_GEM_TEXTURE"));
			//cout << "P" << endl;
		}
	}
}

/*
 * Display message card sprite depening on terminal state
 */
void WitchGame::updateCard()
{
	if (m_player->getGems() < 0)
		m_card->SetTexture(m_tm.getTexture("MSG_LOSE_TEXTURE"));
	if (m_player->getLevelDone())
		m_card->SetTexture(m_tm.getTexture("MSG_WIN_TEXTURE"));
}

/*
 * Execute main program and game loop
 */ 
void WitchGame::runGame(WinCanvas &w, InputState &i)
{
	// Execute program loop
	while (!programFinished)
	{
		runSplashScreen(w, i); // Display title screen
		if (programFinished)
			break;

		// Initialise sprites and data
		initMazeBlocks();
		initDoor();
		initPlayer(i);
		initGems(PURPLE);
		initGems(ORANGE);
		initGems(GREEN);
		initEnemies();
		initScoreboard();
		initCard();
		
		/*
		* Execute the game loop
		*/

		gamefinished = false;

		// The main game loop
		while (!gamefinished)
		{
			// ****************************************************************		
			// *** STAGE 1: Check InputState										***
			// ****************************************************************		
		
			if (m_player->getGems() >= 0 && !m_player->getLevelDone())
			{
				if (i.isActive(VK_ESCAPE))		// Check to see if the Game Loop should terminated
				{
					gamefinished = true;	// Break out of this game loop
					continue;
				}
			}

			// if terminal state of game reached
			if (m_player->getGems() < 0 || m_player->getLevelDone())
			{
				if (i.isActive(VK_RETURN))
				{
					gamefinished = true; // Finish game, go back to title screen
					continue;
				}
				if (i.isActive(VK_X)) 
				{
					gamefinished = true;	// Break out of this game loop
					programFinished = true;
					w.Terminate();			// Force a quite message to be passed to the 
											// message handler, which will close that thread
					continue;
				}
			}

			m_world.OnInput();


			// ******************************************************************************		
			// *** STAGE 2: Update the state of the scene objects based on the user input ***
			// ******************************************************************************

			m_world.OnUpdate(0.01f);
			updateScoreboard();
			updateCard();


			// ******************************************************************		
			// *** STAGE 3: Draw the shapes & text to the screen			  ***
			// ******************************************************************		
			w.Clear(255, 255, 255);		// Need to clear the old screen before drawing new updated screen!
			m_world.OnDraw(w);				// Call the scene object's draw handler
			w.Display();					// Last step is to actually 'display' the elements to the screen
		}
		// Refresh screen after game ended
		w.Clear(255, 255, 255);
		m_world.Clear();
		m_scoreboard.clear(); // refresh scoreboard
		w.Display();
	}
}

/*
 * Return gem co-ordinate data based on gem colour
 */
vector<POINT2f> WitchGame::getGemsCoords(gem_type t)
{
	vector<POINT2f> result;

	switch (t)
	{
		case PURPLE:
		{
			result = purpleGemsCoords;
			break;
		}
		case GREEN:
		{
			result = greenGemsCoords;
			break;
		}
		default:
		{
			result = orangeGemsCoords;
			break;
		}
	}
	return result;
}

/*
 * Return game level
 */
int WitchGame::getLevel()
{
	return level;
}

/*
 * Set game level
 */
void WitchGame::setLevel(int l)
{
	level = l;
}

/*
* Return score gem co-ordinate & position data
*/
vector<POINT2f> WitchGame::getScoreGems()
{
	return scoreGems;
}

/*
* Load score gem sprite position data from file
*/
vector<POINT2f> WitchGame::loadScoreGems()
{
	vector<POINT2f> data;
	POINT2f temp;

	ifstream scoreGemsFile;
	scoreGemsFile.open("resource//scoregems.dat");
	if (!scoreGemsFile.is_open())
		cout << "scoreGems file error.";
	else 
	{
		while (!scoreGemsFile.eof())
		{
			scoreGemsFile >> temp.x;
			scoreGemsFile >> temp.y;
			data.push_back(temp);
		}
		scoreGemsFile.close();
	}
	
	return data;
}
