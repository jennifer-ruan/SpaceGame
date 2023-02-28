//------------------------------------------------------------------------
// Player.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "App/app.h";
#include "Player.h"
//------------------------------------------------------------------------

using namespace std;

enum
{
	ANIM_WALK,
	ANIM_IDLE,
	ANIM_UNALIVE,
};

Player::Player(float x, float y) {
	m_xPos = x, m_yPos = y;
	m_sprite = App::CreateSprite(".\\Assets\\Art\\Character.bmp", 8, 9);
	m_sprite->SetPosition(x, y);
	m_hitbox = CreateCollider(m_xPos, m_yPos, playerWidth, playerHeight, PLAYER, playerXOffset, playerYOffset);
	m_sprite->CreateAnimation(ANIM_IDLE, m_animSpeed,
		{ 0, 1, 0, 1, 0, 1, 0, 1 });
	m_sprite->CreateAnimation(ANIM_WALK, m_animSpeed,
		{16, 17, 18, 19, 16, 17, 18, 19});
	m_sprite->CreateAnimation(ANIM_UNALIVE, m_animSpeed,
		{ 47, 48, 49, 47, 48, 49 });
	m_sprite->SetScale(2.0f);
}

void Player::Input(CController controller, std::vector<Collider> obstacles) {
	if (m_isAlive) {
		if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_UP, false)) {
			Shoot(UP);
		}
		else if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false)) {
			Shoot(DOWN);
		}
		else if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false)) {
			Shoot(LEFT);
		}
		else if (controller.CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false)) {
			Shoot(RIGHT);
		}

		if (abs(controller.GetLeftThumbStickX()) > 0.5 || abs(controller.GetLeftThumbStickY()) > 0.5) {
			m_sprite->SetAnimation(ANIM_WALK);
			Move(controller.GetLeftThumbStickX(), controller.GetLeftThumbStickY());
		}
		else {
			m_sprite->SetAnimation(ANIM_IDLE);
		}
	}
}

void Player::Move(float x, float y) {
	if (x > 0.5f) { 
		m_xPos += m_speed;
	}
	else if (x < -0.5f) {
		m_xPos -= m_speed;
	}
	if (y < -0.5f) {
		m_yPos += m_speed;
	}
	else if (y > 0.5f) {
		m_yPos -= m_speed;
	}
	m_sprite->SetPosition(m_xPos, m_yPos);
}

void Player::Shoot(int direction) {
	if ((time(0) - m_timeLastShot) >= shootingCooldown) {
		App::PlaySound(".\\Assets\\Audio\\Effects\\Shot.wav");
		Projectile* bullet = new Projectile(m_bullets.size(), m_xPos, m_yPos, direction);
		m_bullets.push_back(bullet);
		m_timeLastShot = time(0);
	}
}

std::vector<Collider> Player::GetColliders() {
	std::vector<Collider> colliders;
	colliders.push_back(m_hitbox);
	for (int i = 0; i < m_bullets.size(); i++) {
		colliders.push_back(m_bullets[i]->GetCollider());
	}
	return colliders;
}

//------------------------------------------------------------------------
// This is a separate function from Unalive() because I've tried to the
//  best of my abilities to ensure every function only has a singular purpose.
//------------------------------------------------------------------------
void Player::TakeDamage(int damage) {
	App::PlaySound(".\\Assets\\Audio\\Effects\\Damage.wav");
	m_health = max(0, m_health - damage);

	if (m_health == 0) {
		Unalive();
	}
}

void Player::Unalive() {
	m_isAlive = false;
	m_sprite->SetAnimation(ANIM_UNALIVE);
}

void Player::OnCollision(Collider otherCollider) {
	float playerLeft = m_hitbox.xMin, playerBottom = m_hitbox.yMin, playerRight = m_hitbox.xMax, playerTop = m_hitbox.yMax;
	float colliderLeft = otherCollider.xMin, colliderBottom = otherCollider.yMin, colliderRight = otherCollider.xMax, colliderTop = otherCollider.yMax;
	float distanceFromLeft = abs(playerRight - colliderLeft), distanceFromRight = abs(playerLeft - colliderRight), 
		distanceFromBottom = abs(playerTop - colliderBottom), distanceFromTop = abs(playerBottom - colliderTop);
	switch (otherCollider.tag) {
		case ITEM:
			App::PlaySound(".\\Assets\\Audio\\Effects\\Pickup.wav");
			m_dropsCollected += 1;
			break;
		case ENEMY:
			TakeDamage(1);
			break;
		case OBSTACLE:
		case BOUNDS:
			float minDistance = min(distanceFromLeft, min(distanceFromRight, min(distanceFromBottom, distanceFromTop)));
			if(minDistance == distanceFromLeft) {
				m_xPos = colliderLeft - playerWidth/2 + playerXOffset;
			}
			else if (minDistance == distanceFromRight) {
				m_xPos = colliderRight + playerWidth / 2 - playerXOffset;
			}
			else if (minDistance == distanceFromTop) {
				m_yPos = colliderTop + playerHeight/2 - playerYOffset;
			}
			else {
				m_yPos = colliderBottom - playerHeight / 2 + playerYOffset;
			}
			break;
		}
}

void Player::Update(float dt, std::vector<Collider> colliders) {
	std::vector<Collider> collisions = GetCollisions(m_hitbox, colliders);
	for (int i = 0; i < collisions.size(); i++) {
		OnCollision(collisions[i]);
	}
	m_hitbox.xMin = m_xPos - 18;
	m_hitbox.xMax = m_xPos + 16;
	m_hitbox.yMin = m_yPos - 30;
	m_hitbox.yMax = m_yPos + 24;

	std::vector<Projectile*> remainingBullets;
	for (int i = 0; i < m_bullets.size(); i++) {
		if (!m_bullets[i]->GetIsKilled()) {
			remainingBullets.push_back(m_bullets[i]);
		}
		m_bullets[i]->Update(dt, colliders);
	}
	m_bullets = remainingBullets;

	m_sprite->Update(dt);
}

void Player::Draw() {
	m_sprite->Draw();

	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i]->Draw();
	}

	//DrawCollider(m_hitbox);
}

void Player::Shutdown() {
	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i]->Shutdown();
	}
	delete m_sprite;
	m_bullets.clear();
}