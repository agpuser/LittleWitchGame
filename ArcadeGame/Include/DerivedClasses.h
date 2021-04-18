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

#ifndef DERIVED_CLASS_H
#define DERIVED_CLASS_H

#include "BaseClasses.h"
#define SPRITE_DIM 40 // sprite "collision rectangle" dimension

// Data type for game gem sprites
// Each colour gem has unique functionality
enum gem_type {
	PURPLE,
	GREEN,
	ORANGE
};


class Block : public Sprite, public Dynamic, public Rect
{
	private:
		// Implement inherited private implementation
		virtual void draw_impl(Canvas & c);
		virtual void update_impl(float const & dt);

	public:
		Block(unsigned int w, unsigned int h, POINT2f const & p = { 0, 0 });
		~Block();

		int GetWidth();
		int GetHeight();

		void SetWidth(int w);
		void SetHeight(int h);

};

class TexturedSprite : public Sprite
{
	private:
		TEXTURE_PTR  m_pTexture;

		virtual void draw_impl(Canvas & c);

	protected:

	public:
		explicit TexturedSprite(TEXTURE_PTR pt = nullptr, POINT2f pos = { 0, 0 });

		virtual ~TexturedSprite(){}

		void SetTexture(TEXTURE_PTR pt);
		TEXTURE_PTR GetTexture();
};

/*
 * Base class for WitchGame sprites
 * Class declaration
 */
class GameSprite : public TexturedSprite
{
private:
	TEXTURE_PTR left_mpTexture; // "left facing" sprite texture
	TEXTURE_PTR right_mpTexture; // "right facing" sprite texture
protected:
	VECTOR2f direction; // sprite movement direction
	float speed; // Step increment for sprite movement
	float offset; // Modifier use for collision bounds calculations
public:
	GameSprite(
		TEXTURE_PTR pt, POINT2f pos); //TexturedSprite parameters
	~GameSprite();
	bool pointIsBound(POINT2f p);
	bool hasCollided(POINT2f p, int w, int h);
	float getSpeed();
	void setSpeed(float s);
	VECTOR2f getDirection();
	void setDirection(VECTOR2f d);
	void SetRightTexture(TEXTURE_PTR pt);
	void SetLeftTexture(TEXTURE_PTR pt);
	TEXTURE_PTR GetRightTexture();
	TEXTURE_PTR GetLeftTexture();
	float getOffset();
	void setOffset(float o);
	int getWidth();
	int getHeight();
};

/*
* Derived class for WitchGame player sprite
* Class declaration
*/
class PlayerCharacter : public GameSprite, public Controllable
{
private:
	int gems; // current no. of gems held by player
	bool levelDone; // flag for whether level has been completed
	TEXTURE_PTR m_ghostTexture;
	TEXTURE_PTR m_cupTexture;
		
public:
	PlayerCharacter(
		TEXTURE_PTR pt, POINT2f pos, //TexturedSprite parameters
		InputState * pControl = nullptr); //Controllable parameters
	~PlayerCharacter();
	void input_impl();
	virtual void OnInput();
	void setGems(int g);
	int getGems();
	void modifyGems(int m);
	bool getLevelDone();
	void setLevelDone(bool ld);
	void setGhostTexture(TEXTURE_PTR gt);
	TEXTURE_PTR getGhostTexture();
	void setCuptTexture(TEXTURE_PTR ct);
	TEXTURE_PTR getCupTexture();
};

/*
* Derived class for WitchGame ghost enemy sprites
* Class declaration
*/
class EnemyCharacter : public GameSprite, public Dynamic
{
private:
	bool isFrozen; // flag for whether sprite is not moving
	POINT2f direction; // sprite direction vector
	virtual void update_impl(float const & dt);
public:
	EnemyCharacter(
		TEXTURE_PTR pt, POINT2f pos, //TexturedSprite parameters
		VECTOR2f const & v = { 0, 0 }); //Dynamic parameters
	~EnemyCharacter();
	virtual void OnUpdate();
	POINT2f getDirection();
	void setDirection(POINT2f d);
	bool isEnemyFrozen();
	void setEnemyFrozen(bool f);
};

/*
* Derived class for WitchGame gem sprites
* Class declaration
*/
class Gem : public GameSprite
{
private:
	gem_type type;
	
public:
	Gem(gem_type t,
		TEXTURE_PTR pt, POINT2f pos); //TexturedSprite parameters
	~Gem() {}
	gem_type getType();
	void setType(gem_type t);
};

// Game sprite pointer aliases
using PLAYER_PTR = std::shared_ptr<PlayerCharacter>;
using ENEMY_PTR = std::shared_ptr<EnemyCharacter>;
using MAZE_PTR = std::shared_ptr<GameSprite>;
using DOOR_PTR = std::shared_ptr<GameSprite>;
using GEM_PTR = std::shared_ptr<Gem>;

#endif