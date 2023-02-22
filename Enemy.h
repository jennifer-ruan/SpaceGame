//------------------------------------------------------------------------
// Enemy.h
//------------------------------------------------------------------------
#ifndef _ENEMY_H
#define _ENEMY_H
//---------------------------------------------------------------------------------
#include <math.h>  
//---------------------------------------------------------------------------------
#include "app\app.h"
#include "Collision.h"
//---------------------------------------------------------------------------------

static const int enemyHeight = 48;
static const int enemyWidth = 28;
static const int enemyXOffset = 0;
static const int enemyYOffset = -7;

class Enemy {
public:
	Enemy(float x, float y);
	Collider GetCollider() { return m_hitbox; };
	void GetPosition(float& x, float& y) { x = m_xPos; y = m_yPos; };
	bool GetIsKilled() { return m_isKilled; };
	void Update(float dt, std::vector<Collider> colliders, float playerXPos, float playerYPos); // Here I pass the player's x and y positions rather than the object to reduce dependencies.
	void Draw();
	void Shutdown();

private:
	float m_xPos = 0.0f;
	float m_yPos = 0.0f;
	float m_speed = 0.0f;
	float m_animSpeed = 1.0f / 10.0f;
	bool m_isKilled = false;
	Collider m_hitbox{};
	CSimpleSprite* m_sprite;
	void Move(float playerXPos, float playerYPos); // Enemies will move towards the player
	void OnCollision(Collider otherCollider);
};

#endif