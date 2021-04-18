/* ********************************************************************************* *
 * *  COPYRIGHT NOTICE                                                             * *
 * *  ----------------                                                             * *
 * *  (C)[2012] - [2015] Deakin University                                         * *
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

#include "Scene.h"
#include "windows.h"
#include <iostream>

using namespace std;

Scene::Scene(POINT2 const & s, POINT2 const & e)
	: m_ul(s),
	  m_br(e),
	  m_game_entities(),
	  m_drawable_objects(),
	  m_dynamic_objects(),
	  m_controlled_objects()
{}

Scene::~Scene()
{
	Clear();
}

void Scene::OnInput()
{
	for (auto c : m_controlled_objects)
		c->OnInput();
}

void Scene::OnUpdate(float const & dt)
{
	// Update any sprite object that requires it
	for (auto d : m_dynamic_objects)
		d->OnUpdate(dt);

	// resolve collisions
	RunCollisionTests();

	// do any other update actions (such as remove dead sprites)
}

void Scene::OnDraw(Canvas & c)
{
	for (auto s : m_drawable_objects)
		s->OnDraw(c);
}

bool Scene::Load(std::string const & filename)
{
	/*
	 * Add functionality here to load a set of sprites from the given file
	 */


	return false; // if failed to load
}
void Scene::AddEntity(ENTITY_PTR pEntity)
{
	m_game_entities.insert( std::make_pair(pEntity->GetUID(), pEntity) );
}

void Scene::AddSprite(SPRITE_PTR pSprite)
{
	m_drawable_objects.push_back(pSprite);
}

void Scene::AddDynamic(DYNAMIC_PTR pDynamic)
{
	m_dynamic_objects.push_back(pDynamic);
}

void Scene::AddControlled(CONTROL_PTR pControlled)
{
	m_controlled_objects.push_back(pControlled);
}

// Add player sprite to Scene collections
void Scene::AddPlayer(PLAYER_PTR pPlayer)
{
	m_player.push_back(pPlayer);
}

// Add enemy sprites to Scene collections
void Scene::AddEnemy(ENEMY_PTR pEnemy)
{
	m_enemies.push_back(pEnemy);
}

// Add maze block sprite to Scene collections
void Scene::AddMazeBlock(MAZE_PTR pMaze)
{
	m_maze_blocks.push_back(pMaze);
}

// Add gem sprites to Scene collections
void Scene::AddGem(GEM_PTR pGem)
{
	m_gems.push_back(pGem);
}

// Add door sprite to Scene collections
void Scene::AddDoor(DOOR_PTR pDoor)
{
	m_door.push_back(pDoor);
}

ENTITY_PTR Scene::RemoveEntity(UID_TYPE const & uid)
{
	ENTITY_PTR p = nullptr;
	auto iter = m_game_entities.find(uid);
	if (iter != m_game_entities.end())
	{
		p = Find(uid);
		m_game_entities.erase(iter);
	}
	return p;
}

void Scene::RemoveSprite(SPRITE_PTR pSprite)
{
	m_drawable_objects.remove(pSprite);
}
void Scene::RemoveDynamic(DYNAMIC_PTR pDynamic)
{
	m_dynamic_objects.remove(pDynamic);
}


ENTITY_PTR Scene::Find(UID_TYPE const & uid)
{
	auto iter = m_game_entities.find(uid);
	if (iter != m_game_entities.end())
		return iter->second;
	else
		return nullptr;
}

void Scene::Clear()
{
	m_game_entities.clear();
	m_drawable_objects.clear();
	m_dynamic_objects.clear();
	m_controlled_objects.clear();
	m_player.clear();
	m_enemies.clear();
	m_maze_blocks.clear();
	m_gems.clear();
	m_door.clear();
}

// Handle sprite cpllisions
// Obtain sprites from Scene collection storage
void Scene::RunCollisionTests()
{
	GEM_PTR removeGem; // variable to save a removed gem
	for (auto p : m_player) // Obtain player sprite
	{
		VECTOR2f pd = p->getDirection(); // Save player direction
		POINT2f pp = p->GetPosition(); // Save player position
		
		// Loop and obtain each maze black sprite from scene
		for (auto mb : m_maze_blocks)
		{
			if (mb->hasCollided(p->GetPosition(), SPRITE_DIM, SPRITE_DIM)) // Player collides with maze block
			{
				// Set player position back to previous position
				p->SetPosition(p->GetPosition() + POINT2f((pd.x * -1) * p->getSpeed(), (pd.y * -1) * p->getSpeed()));
			}

			// Loop and obtain each ghost enemy sprite from scene
			for (auto e : m_enemies)
			{
				VECTOR2f ed = e->GetVelocity(); // save ghost velocity
				POINT2f mp = mb->GetPosition(); // save maze block position
				POINT2f ep = e->GetPosition(); // save enemy position

				// Enemy vs. Maze Block collision check
				if (mb->hasCollided(ep, SPRITE_DIM, SPRITE_DIM))
				{
					// Reverse enemy velocity which reverses direction
					ed.x *= -1.0;
					ed.y *= -1.0;
					e->SetVelocity(ed);
				}

				// Player vs. Enemy collision check
				if (e->hasCollided(pp, SPRITE_DIM, SPRITE_DIM))
				{
					if (p->getGems() <= 0) // Player has "died"
					{
						// set player ghost texture, move off maze
						//Sleep(500);
						p->SetTexture(p->getGhostTexture());
						p->SetPosition(POINT2f(700.0, 300.0));
						p->setGems(-1); // set gems to -1 as "Restart" flag
					}
					else
					{
						Sleep(500);
						pp.x = 500.0;
						pp.y = 500.0;
						p->SetTexture(p->GetLeftTexture());
						p->SetPosition(pp); // move player to start position
						p->modifyGems(-1); // lose one gem
					}
				}
			}
		}
		
		//Obtain gem sprites from Scene
		for (auto g : m_gems)
		{
			POINT2f mp = g->GetPosition();
			// Player vs. Gems collision check
			if (g->hasCollided(pp, SPRITE_DIM, SPRITE_DIM))
			{
				switch(g->getType())
				{
					case PURPLE:
					{
						//m_gems.remove(g);
						RemoveSprite(g); // remove gem sprite
						removeGem = g; // save gem;
						p->modifyGems(1); // Player gains a gem
						break;
					}
					case GREEN:
					{
						// Freeze enemies
						RemoveSprite(g);
						removeGem = g;
						freezeEnemies();
						break;
					}
					default: // ORANGE gem
					{
						// Remove enemies
						RemoveSprite(g);
						removeGem = g;
						removeEnemies();
						break;
					}
				}
			}
		}
		// Remove saved gem from list
		m_gems.remove(removeGem);

		//Obtain door sprite from Scene
		for (auto d : m_door)
		{
			POINT2f dp = d->GetPosition();
			// Player vs. Door collision check
			if (d->hasCollided(pp, SPRITE_DIM, SPRITE_DIM))
			{
				//m_gems.remove(g);
				Sleep(500); // dramatic pause
				p->setLevelDone(true);
				p->SetTexture(p->getCupTexture()); // switch to winning "cup" texture
				pp.x = 645.0;
				pp.y = 245.0;
				p->SetPosition(pp); // move player off maze
			}
		}
	}
}

// Make enemy ghostd stationary
void Scene::freezeEnemies()
{
	for (auto e : m_enemies)
	{
		e->setEnemyFrozen(true);
		e->SetVelocity(POINT2f(0.0, 0.0));
	}
}

// Taks all ghost enemies out of the maze
void Scene::removeEnemies()
{
	for (auto e : m_enemies)
	{
		RemoveSprite(e);
		RemoveDynamic(e);
	}
	m_enemies.clear();
}

PLAYER_PTR Scene::getPlayer()
{
	return m_player.front();
}
