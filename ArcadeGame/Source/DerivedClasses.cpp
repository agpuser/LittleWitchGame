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


#include "DerivedClasses.h"
#include "WinCanvas.h"
#include <iostream>

using namespace std;

Block::Block(unsigned int w, unsigned int h, POINT2f const & p)
: Sprite(p), Dynamic(), Rect(p, POINT2f(p.x+w,p.y+h))
	{}

Block::~Block()
{}

/*
 *		Implementation of Block update, invoked
 *		through base class (Sprite) update method.
 */
void Block::update_impl(float const & dt)
{
	VECTOR2f d = br - ul;
	ul = m_position;
	br.x = m_position.x + d.x;
	br.y = m_position.y + d.y;
	SetPosition((GetPosition() + GetVelocity()));
}

/*
*		The Block implementaiton method for draw, which is called
*		from the base object (Shape) draw method.
*/
void Block::draw_impl(Canvas & c)
{
	// Use the GraphicsCanvas API to draw a solid Block in 'RED'
	c.DrawSolidRect(*this, RGB(0, 255, 0));
}



int Block::GetWidth()
{

	return (int)(br.x - ul.x);
}
int Block::GetHeight()
{
	return (int)(br.y - ul.y);
}

void Block::SetWidth(int w)
{
	br.x = ul.x + w;
}
void Block::SetHeight(int h)
{
	br.y = ul.y + h;
}



TexturedSprite::TexturedSprite(TEXTURE_PTR pt, POINT2f pos)
: Sprite(pos),
  m_pTexture(pt)  
{}

void TexturedSprite::draw_impl(Canvas & c)
{
	c.DrawTexture(*m_pTexture, m_position);
}
void TexturedSprite::SetTexture(TEXTURE_PTR pt)
{
	m_pTexture = pt;
}

TEXTURE_PTR TexturedSprite::GetTexture()
{
	return m_pTexture;
}

GameSprite::GameSprite(
	TEXTURE_PTR pt, POINT2f pos) //TexturedSprite parameters
	:TexturedSprite(pt, pos)
{
	speed = (float)0.2;
	offset = (float)0.0;
}

GameSprite::~GameSprite() {}

// Return whether a point is within this sprite's 4 corner bounds
bool GameSprite::pointIsBound(POINT2f p)
{ 
	bool result = false;
	POINT2f tl = m_position;
	POINT2f br = POINT2f(tl.x + GetTexture()->Width(), tl.y + GetTexture()->Height());
	if (p.x > (tl.x+offset) && p.x < (br.x-offset) && p.y > (tl.y+offset) && p.y < (br.y-offset))
		result = true;
	return result;
}

// Return whether a sprite has a point within this sprite's bounds
bool GameSprite::hasCollided(POINT2f p, int w, int h)
{
	bool result = false;
	
	if (pointIsBound(p) ||
		pointIsBound(POINT2f(p.x + w, p.y)) ||
		pointIsBound(POINT2f(p.x, p.y + h)) ||
		pointIsBound(POINT2f(p.x + w, p.y + h)))
		result = true;
	return result; 
}

// Return sprite speed
float GameSprite::getSpeed()
{
	return speed;
}

// Set sprite speed
void GameSprite::setSpeed(float s)
{
	speed = s;
}

// Return sprite direction
VECTOR2f GameSprite::getDirection()
{
	return direction;
}

// Set sprite direction
void GameSprite::setDirection(VECTOR2f d)
{
	direction = d;
}

// Accessor methods
TEXTURE_PTR GameSprite::GetRightTexture()
{
	return right_mpTexture;
}

TEXTURE_PTR GameSprite::GetLeftTexture()
{
	return left_mpTexture;
}

float GameSprite::getOffset()
{
	return offset;
}

int GameSprite::getWidth()
{
	TEXTURE_PTR t = GetTexture();
	return t->Width();
}

int GameSprite::getHeight()
{
	TEXTURE_PTR t = GetTexture();
	return t->Height();
}

// Mutator methods
void GameSprite::SetRightTexture(TEXTURE_PTR pt)
{
	right_mpTexture = pt;
}

void GameSprite::SetLeftTexture(TEXTURE_PTR pt)
{
	left_mpTexture = pt;
}


void GameSprite::setOffset(float o)
{
	offset = 0;
}



PlayerCharacter::PlayerCharacter(
	TEXTURE_PTR pt, POINT2f pos, //TexturedSprite parameters
	InputState * pControl) //Controllable parameters
	: GameSprite(pt, pos), Controllable(pControl)
{
	gems = 0;
	offset = 2.0; // defualt bounds modifier value
	levelDone = false; // default set to level not done
}

PlayerCharacter::~PlayerCharacter() {}
void PlayerCharacter::input_impl()
{

}

// Respond to game input by changing player
void PlayerCharacter::OnInput()
{
	if (gems >= 0 && !levelDone) // if game not at terminal state
	{
		if (m_pInput->isActive(VK_UP))
		{
			direction.x = 0;
			direction.y = -1;
			SetPosition(GetPosition() + VECTOR2f(direction.x * speed, direction.y * speed));
		}
		else if (m_pInput->isActive(VK_DOWN))
		{
			direction.x = 0;
			direction.y = 1;
			SetPosition(GetPosition() + VECTOR2f(direction.x * speed, direction.y * speed));
		}
		else if (m_pInput->isActive(VK_LEFT))
		{
			direction.x = -1;
			direction.y = 0;
			SetPosition(GetPosition() + VECTOR2f(direction.x * speed, direction.y * speed));
			SetTexture(GetLeftTexture());
		}
		else if (m_pInput->isActive(VK_RIGHT))
		{
			direction.x = 1;
			direction.y = 0;
			SetPosition(GetPosition() + VECTOR2f(direction.x * speed, direction.y * speed));
			SetTexture(GetRightTexture());
		}
	}
}

//Accessor methods
int PlayerCharacter::getGems()
{
	return gems;
}

bool PlayerCharacter::getLevelDone()
{
	return levelDone;
}

TEXTURE_PTR PlayerCharacter::getGhostTexture()
{
	return m_ghostTexture;
}

TEXTURE_PTR PlayerCharacter::getCupTexture()
{
	return m_cupTexture;
}

void PlayerCharacter::modifyGems(int m)
{
	gems += m;
}

// Mutator methods
void PlayerCharacter::setGems(int g)
{
	gems = g;
}

void PlayerCharacter::setLevelDone(bool ld)
{
	levelDone = ld;
}

void PlayerCharacter::setGhostTexture(TEXTURE_PTR gt)
{
	m_ghostTexture = gt;
}

void PlayerCharacter::setCuptTexture(TEXTURE_PTR ct)
{
	m_cupTexture = ct;
}

EnemyCharacter::EnemyCharacter(
	TEXTURE_PTR pt, POINT2f pos, //TexturedSprite parameters
	VECTOR2f const & v) //Dynamic parameters
	: GameSprite(pt, pos), Dynamic(v)
{
	isFrozen = false; // defualt not frozen
	direction.x = v.x;
	direction.y = v.y;
}

EnemyCharacter::~EnemyCharacter() {}

// Update enemy sprite position based on velocity
void EnemyCharacter::update_impl(float const & dt) 
{
	SetPosition(GetPosition() + GetVelocity());
}

void EnemyCharacter::OnUpdate() {}

// Return enemy direction
POINT2f EnemyCharacter::getDirection()
{
	return direction;
}

// Set enemy direction
void EnemyCharacter::setDirection(POINT2f d)
{
	direction = d;
}

// Return whether ghost is frozen
bool EnemyCharacter::isEnemyFrozen()
{
	return isFrozen;
}

// Set ghost to be frozen
void EnemyCharacter::setEnemyFrozen(bool f)
{
	 isFrozen = f;
}

Gem::Gem(gem_type t,
	TEXTURE_PTR pt, POINT2f pos)
	: GameSprite(pt, pos)
{
	type = t;
}

// Return gem type
gem_type Gem::getType()
{
	return type;
}

// Set gem type
void Gem::setType(gem_type t)
{
	type = t;
}


