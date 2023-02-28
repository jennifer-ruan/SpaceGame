//------------------------------------------------------------------------
// Item.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <vector>
//------------------------------------------------------------------------
#include "app\app.h"
#include "Item.h"
//------------------------------------------------------------------------

Item::Item(float x, float y) {
	m_xPos = x, m_yPos = y;
	m_sprite = App::CreateSprite(".\\Assets\\Art\\Items.bmp", 16, 22);
	m_sprite->SetPosition(x, y);
	m_sprite->SetFrame(273);
	m_hitbox = CreateCollider(x, y, 16, 16, ITEM);
}

void Item::OnCollision(Collider otherCollider) {
	switch (otherCollider.tag) {
		case PLAYER:
			m_isCollected = true;
			break;
	}
}


void Item::Update(float dt, std::vector<Collider> colliders) {
	for (int i = 0; i < colliders.size(); i++) {
		if (m_hitbox.xMax >= colliders[i].xMin && m_hitbox.xMin <= colliders[i].xMax && m_hitbox.yMax >= colliders[i].yMin && m_hitbox.yMin <= colliders[i].yMax) {
			OnCollision(colliders[i]);
		}
	}
	m_sprite->Update(dt);
}

void Item::Draw() {
	m_sprite->Draw();
	//DrawCollider(m_hitbox);
}

void Item::Shutdown() {
	delete m_sprite;
}