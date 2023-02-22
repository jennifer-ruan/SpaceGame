//---------------------------------------------------------------------------------
// Item.h
//---------------------------------------------------------------------------------
#ifndef _ITEM_H
#define _ITEM_H
//---------------------------------------------------------------------------------
#include <windows.h> 
#include <vector>
//---------------------------------------------------------------------------------
#include "app\app.h"
#include "Collision.h"
//---------------------------------------------------------------------------------

class Item {
public:
	Item(float x, float y);
	Collider GetCollider() { return m_hitbox; };
	bool GetIsCollected() { return m_isCollected; };
	void Update(float dt, std::vector<Collider> colliders);
	void Draw();
	void Shutdown();
private:
	float m_xPos, m_yPos = 0.0f;
	bool m_isCollected = false;
	CSimpleSprite* m_sprite;
	Collider m_hitbox{};
	void OnCollision(Collider otherCollider);
};

#endif