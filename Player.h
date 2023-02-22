//---------------------------------------------------------------------------------
// Player.h
//---------------------------------------------------------------------------------
#ifndef _PLAYER_H
#define _PLAYER_H

#include <math.h>  
#include <ctime>

#include "app\app.h"
#include "Collision.h"
#include "Projectile.h"

//#define MAX_BULLETS 5

static const int maxHealth = 3;
static const int shootingCooldown = 1;
static const int playerHeight = 56;
static const int playerWidth = 34;
static const int playerXOffset = -2;
static const int playerYOffset = -4;

class Player {
public:
	Player(float x, float y);
	void Input(CController controller, std::vector<Collider> obstacles);
	void TakeDamage(int damage);
	Collider GetCollider() { return m_hitbox; };
	void GetPosition(float& x, float& y) { x = m_xPos; y = m_yPos; };
	int GetHealth() { return m_health; };
	int GetNumDropsCollected() { return m_dropsCollected; };
	int GetIsAlive() { return m_isAlive; };
	std::vector<Collider> GetColliders();
	void Update(float dt, std::vector<Collider> colliders);
	void Draw();
	void Shutdown();

private:
	int m_health = maxHealth;
	int m_dropsCollected = 0;
	float m_xPos = 0.0f;
	float m_yPos = 0.0f;
	float m_speed = 2.0f;
	float m_animSpeed = 1.0f / 5.0f;
	bool m_isAlive = true;
	time_t m_timeLastShot = time(0);
	CSimpleSprite *m_sprite;
	Collider m_hitbox{};
	std::vector<Projectile*> m_bullets;
	void Shoot(int direction);
	void OnCollision(Collider otherCollider);
	void Move(float x, float y);
	void Unalive();
};

#endif