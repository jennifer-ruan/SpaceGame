//------------------------------------------------------------------------
// Enemy.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
//------------------------------------------------------------------------
#include "Enemy.h"
//------------------------------------------------------------------------

enum {
	ANIM_WALK
};

Enemy::Enemy(float x, float y) {
	m_xPos = x, m_yPos = y;
	m_speed = (rand() % 10 / 10) + 1.0f;
	m_sprite = App::CreateSprite(".\\Assets\\Art\\Mushroom.bmp", 8, 1);
	m_sprite->SetPosition(x, y);
	m_hitbox = CreateCollider(x, y, 28, 48, ENEMY);
	m_sprite->CreateAnimation(ANIM_WALK, m_animSpeed,
		{ 1, 2, 3, 4, 5, 6, 7, 8 });
	m_sprite->SetAnimation(ANIM_WALK);
	m_sprite->SetScale(1.25f);
}

void Enemy::OnCollision(Collider otherCollider) {
	float enemyLeft = m_hitbox.xMin, enemyBottom = m_hitbox.yMin, enemyRight = m_hitbox.xMax, enemyTop = m_hitbox.yMax;
	float colliderLeft = otherCollider.xMin, colliderBottom = otherCollider.yMin, colliderRight = otherCollider.xMax, colliderTop = otherCollider.yMax;
	float distanceFromLeft = abs(enemyRight - colliderLeft), distanceFromRight = abs(enemyLeft - colliderRight),
		distanceFromBottom = abs(enemyTop - colliderBottom), distanceFromTop = abs(enemyBottom - colliderTop);
	float minDistance = min(distanceFromLeft, min(distanceFromRight, min(distanceFromBottom, distanceFromTop)));
	switch (otherCollider.tag) {
	case PROJECTILE:
		App::PlaySound(".\\Assets\\Audio\\Effects\\Kill.wav");
		m_isKilled = true;
		break;
	case OBSTACLE:
		if (minDistance == distanceFromLeft) {
			m_xPos = colliderLeft - enemyWidth/2 + enemyXOffset;
		}
		else if (minDistance == distanceFromRight) {
			m_xPos = colliderRight + enemyWidth/2 - enemyXOffset;
		}
		else if (minDistance == distanceFromTop) {
			m_yPos = colliderTop + enemyHeight / 2 - enemyYOffset;
		}
		else {
			m_yPos = colliderBottom - enemyHeight / 2 + enemyYOffset;
		}
		break;
	case PLAYER:
		if (minDistance == distanceFromLeft) {
			m_xPos = (colliderLeft - enemyWidth);
		}
		else if (minDistance == distanceFromRight) {
			m_xPos = (colliderRight + enemyWidth);
		}
		else if (minDistance == distanceFromTop) {
			m_yPos = (colliderTop + enemyHeight);
		}
		else {
			m_yPos = (colliderBottom - enemyHeight);
		}
		break;
	}
}

void Enemy::Move(float playerXPos, float playerYPos) {
	float xDistance = playerXPos - m_xPos, yDistance = playerYPos - m_yPos;
	if (xDistance != 0) { // Have to check that the xDistance and yDistance are not 0 to prevent NaN (dividing a number by 0)
		m_xPos += xDistance / abs(xDistance) * m_speed;
	}
	if (yDistance != 0) {
		m_yPos += yDistance / abs(yDistance) * m_speed;
	}
	m_sprite->SetPosition(m_xPos, m_yPos);

}

void Enemy::Update(float dt, std::vector<Collider> colliders, float playerXPos, float playerYPos) {
	Move(playerXPos, playerYPos);
	m_hitbox = CreateCollider(m_xPos, m_yPos, enemyWidth, enemyHeight, ENEMY, enemyXOffset, enemyYOffset);

	std::vector<Collider> collisions = GetCollisions(m_hitbox, colliders);
	for (int i = 0; i < collisions.size(); i++) {
		OnCollision(collisions[i]);
	}

	m_sprite->Update(dt);
}

void Enemy::Draw() {
	m_sprite->Draw();

	//DrawCollider(m_hitbox);
}

void Enemy::Shutdown() {
	delete m_sprite;
}