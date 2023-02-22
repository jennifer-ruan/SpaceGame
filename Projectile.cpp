//------------------------------------------------------------------------
// Projectile.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
//------------------------------------------------------------------------
#include "App/app.h";
#include "Projectile.h"
//------------------------------------------------------------------------

Projectile::Projectile(int id, float x, float y, int direction){
	m_id = id, m_xPos = x, m_yPos = y, m_direction = direction;
	m_sprite = App::CreateSprite(".\\Assets\\Art\\Projectile.bmp", 1, 1);
	m_sprite->SetScale(0.4f);
	m_hitbox = CreateCollider(x, y, 16, 16, PROJECTILE);
	m_sprite->SetPosition(x, y);
}

void Projectile::OnCollision(Collider otherCollider) {
	switch (otherCollider.tag) {
	case OBSTACLE:
	case BOUNDS:
	case ENEMY:
		m_isKilled = true;
	}
}

void Projectile::Update(float dt, std::vector<Collider> colliders) {
	std::vector<Collider> collisions = GetCollisions(m_hitbox, colliders);
	for (int i = 0; i < collisions.size(); i++) {
		OnCollision(collisions[i]);
	}
	switch (m_direction) {
		case UP:
			m_yPos += m_speed;
			break;
		case DOWN:
			m_yPos -= m_speed;
			break;
		case RIGHT:
			m_xPos += m_speed;
			break;
		case LEFT:
			m_xPos -= m_speed;
			break;
	}
	m_hitbox = CreateCollider(m_xPos, m_yPos, 16, 16, PROJECTILE);
	m_sprite->SetPosition(m_xPos, m_yPos);
	m_sprite->Update(dt);
}

void Projectile::Draw() {
	m_sprite->Draw();
	// DrawCollider(m_hitbox);
}

void Projectile::Shutdown() {
	delete m_sprite;
}