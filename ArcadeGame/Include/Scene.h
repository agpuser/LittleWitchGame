/* ********************************************************************************* *
* *  COPYRIGHT NOTICE                                                             * *
* *  ----------------                                                             * *
* *  (C)[2012] - [2017] Deakin University                                         * *
* *  All rights reserved.                                                         * *
* *  All information contained herein is, and remains the property of Deakin      * *
* *  University and the authors(Tim Wilkin, Michael Hobbs and Shaun Bangay).      * *
* *  Dissemination of this information or reproduction of this material is        * *
* *  strictly forbidden unless prior written permission is obtained from Deakin   * *
* *  University.The right to create derivative works from this material is        * *
* *  hereby granted to students enrolled in SIT153, but only for the purposes of  * *
* *  assessment while an enrolled student at Deakin University.                   * *
* *                                                                               * *
* ********************************************************************************* */

#ifndef SCENE_H
#define SCENE_H
/*
 *		The SCENE object is provided as a starting point for your
 *		implementation of an object that can hold a number of Sprite
 *		objects. Note the use of LIST container, which can hold
 *		SPRITE_PTRs. These are actually std::shared_ptr<Sprite> objects.
 *
 *		You should consider adding methods that would enable managed access
 *		to stored sprites
 */

#include <list>
#include <map>
#include <string>

#include "BaseClasses.h"
#include "DerivedClasses.h"



using ENTITY_LIST = std::map<UID_TYPE, ENTITY_PTR>;
using DRAW_LIST = std::list<SPRITE_PTR>;
using UPDATE_LIST = std::list<DYNAMIC_PTR>;
using CONTROL_LIST = std::list<CONTROL_PTR>;

// Collections of game elements aliases
using PLAYER_LIST = std::list<PLAYER_PTR>;
using ENEMY_LIST = std::list<ENEMY_PTR>;
using MAZE_LIST = std::list<MAZE_PTR>;
using GEM_LIST = std::list<GEM_PTR>;
using DOOR_LIST = std::list<DOOR_PTR>;

class Scene
{
	private:

		ENTITY_LIST		m_game_entities;
		DRAW_LIST		m_drawable_objects;
		UPDATE_LIST		m_dynamic_objects;
		CONTROL_LIST	m_controlled_objects;

		// Declare game element collections
		PLAYER_LIST		m_player;
		ENEMY_LIST		m_enemies;
		MAZE_LIST		m_maze_blocks;
		GEM_LIST		m_gems;
		DOOR_LIST		m_door;

		POINT2			m_ul,
						m_br;

		void RunCollisionTests();

	public:

		Scene(POINT2 const & s, POINT2 const & e);
		~Scene();

		void OnUpdate(float const & dt);
		void OnDraw(Canvas & c);
		void OnInput();

		bool Load(std::string const & filename);
		void Clear();

		ENTITY_PTR Find(UID_TYPE const & uid);

		void AddEntity(ENTITY_PTR pEntity);
		void AddSprite(SPRITE_PTR pSprite);
		void AddDynamic(DYNAMIC_PTR pDynamic);
		void AddControlled(CONTROL_PTR pControlled);

		// WitchGame methods
		void AddPlayer(PLAYER_PTR pPlayer);
		void AddEnemy(ENEMY_PTR pEnemy);
		void AddMazeBlock(MAZE_PTR pMaze);
		void AddGem(GEM_PTR pGem);
		void AddDoor(DOOR_PTR pDoor);

		ENTITY_PTR RemoveEntity(UID_TYPE const & uid);
		void RemoveSprite(SPRITE_PTR pSprite);
		void RemoveDynamic(DYNAMIC_PTR pDynamic);
		void RemoveControlled(CONTROL_PTR pControlled);
		void RemovePlayer(PLAYER_PTR pPlayer);

		// WitchGame methods
		void freezeEnemies();
		void removeEnemies();
		PLAYER_PTR getPlayer();

};

#endif