//---------------------------------------------------------------------------------
// Projectile.h
//---------------------------------------------------------------------------------
#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include <math.h>  

#include "app\app.h"
#include "Collision.h"

enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Projectile {
public:
	Projectile(int id, float x, float y, int direction);
	bool GetIsKilled() { return m_isKilled; };
	Collider GetCollider() { return m_hitbox; };
	void Update(float dt, std::vector<Collider> colliders);
	void Draw();
	void Shutdown();
private:
	int m_id = 0;
	int m_direction = UP;
	float m_xPos = 0;
	float m_yPos = 0;
	float m_speed = 5.0f;
	bool m_isKilled = false;
	CSimpleSprite *m_sprite;
	Collider m_hitbox{};
	void OnCollision(Collider otherColliders);
};

#endif