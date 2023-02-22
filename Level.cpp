//------------------------------------------------------------------------
// Level.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <vector>
//------------------------------------------------------------------------
#include "app\app.h"
#include "Level.h"
//------------------------------------------------------------------------

Level::Level() {
	//Create outer walls (to prevent player and projectiles from existing outside of the window)
	Collider bottomBoundary, topBoundary, leftBoundary, rightBoundary;
	bottomBoundary = CreateCollider(windowWidth / 2, 0, windowWidth, 1, BOUNDS);
	topBoundary = CreateCollider(windowWidth / 2, windowHeight, windowWidth, 1, BOUNDS);
	leftBoundary = CreateCollider(0, windowHeight/2, 1, windowHeight, BOUNDS);
	rightBoundary = CreateCollider(windowWidth, windowHeight/2, 1, windowHeight, BOUNDS);
	m_obstacles.push_back(bottomBoundary);
	m_obstacles.push_back(topBoundary);
	m_obstacles.push_back(leftBoundary);
	m_obstacles.push_back(rightBoundary);

	//Some randomized values so that players' experience variety between planets
	m_enemySpawnInterval = (rand() % 10 / 10) + 1.0f; //Enemy spawn interval is randomized between 1.0f and 2.0f;
	m_wallSpawnChance = rand() % 5 + 1; //Anywhere from 1-5% of the map will be wall tiles

	//Fill in tiles with either random grass tiles or walls
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			if (rand() % (100/m_wallSpawnChance) == 0) {
				m_tiles[i][j] = GetRandomTile(WALL); // Create a wall tile that players, projectiles and enemies can't walk through
				Collider obstacle = CreateCollider(32*j, 32*i, 32, 32, OBSTACLE);
				m_obstacles.push_back(obstacle);
			}
			else {
				m_tiles[i][j] = GetRandomTile(GRASS); // Create a grass tile
			}
			m_tiles[i][j].tileSprite->SetPosition(32 * j, 32 * i);
		}
	}
}

Tile Level::GetRandomTile(tileType type) {
	Tile newTile{};
	
	switch (type) {
		case WALL:
			newTile.tileSprite = App::CreateSprite(".\\Assets\\Art\\Wall.bmp", 10, 4);
			newTile.tileSprite->SetFrame(rand() % numWallTiles);
			newTile.tileSprite->SetScale(2.0f);
			newTile.collision = true;
			break;
		case GRASS:
			//char fileName[25];
			//sprintf(fileName, ".\\TestData\\Grass%d.bmp", m_background);
			//newTile.tile_sprite = App::CreateSprite(fileName, 16, 16);
			newTile.tileSprite = App::CreateSprite(".\\Assets\\Art\\Grass.bmp", 16, 16);
			newTile.tileSprite->SetFrame(rand() % numGrassTiles);
			newTile.tileSprite->SetScale(2.0f);
			newTile.collision = false;
			break;
	}
	return newTile;
}

std::vector<Collider> Level::GetColliders() {
	std::vector<Collider> colliders;
	for (int i = 0; i < m_enemies.size(); i++) {
		colliders.push_back(m_enemies[i]->GetCollider());
	}
	for (int i = 0; i < m_drops.size(); i++) {
		colliders.push_back(m_drops[i]->GetCollider());
	}
	return colliders;
}

void Level::SpawnEnemy() {
	if (time(0) - m_timeEnemyLastSpawned >= m_enemySpawnInterval) {
		float spawnX = 0;
		float spawnY = 0;
		switch (rand() % 4) {
			case 0:
				spawnX = 0 - enemyWidth;
				spawnY = (rand() % windowHeight);
				break;
			case 1:
				spawnX = windowWidth;
				spawnY = (rand() % windowHeight);
				break;
			case 2:
				spawnX = (rand() % windowWidth);
				spawnY = 0 - enemyHeight;
				break;
			case 3:
				spawnX = (rand() % windowWidth);
				spawnY = windowHeight;
				break;
		}
		Enemy* enemy = new Enemy(spawnX, spawnY);
		m_timeEnemyLastSpawned = time(0);
		m_enemies.push_back(enemy);
	}
}

void Level::Update(float dt, std::vector<Collider> colliders, float playerXPos, float playerYPos) {
	SpawnEnemy();
	// Remove projectiles that have collided with enemies or walls
	std::vector<Item*> remainingDrops;
	for (int i = 0; i < m_drops.size(); i++) {
		m_drops[i]->Update(dt, colliders);
		if (!m_drops[i]->GetIsCollected()) {
			remainingDrops.push_back(m_drops[i]);
		}
	}
	m_drops = remainingDrops;
	// Check collisions against the enemy and remove any that are killed
	std::vector<Enemy*> remainingEnemies;
	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->Update(dt, colliders, playerXPos, playerYPos);
		if (!m_enemies[i]->GetIsKilled()) {
			remainingEnemies.push_back(m_enemies[i]);
		}
		else {
			float x, y;
			m_enemies[i]->GetPosition(x, y);
			Item* drop = new Item(x, y);
			m_drops.push_back(drop);
		}
	}
	m_enemies = remainingEnemies;
}

void Level::Draw() {
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < tileCols; j++) {
			m_tiles[i][j].tileSprite->Draw();
		}
	}
	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->Draw();
	}
	for (int i = 0; i < m_obstacles.size(); i++) {
		DrawCollider(m_obstacles[i]);
	}
	for (int i = 0; i < m_drops.size(); i++) {
		m_drops[i]->Draw();
	}
}

void Level::Shutdown() {
	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->Shutdown();
	}
	for (int i = 0; i < m_drops.size(); i++) {
		m_drops[i]->Shutdown();
	}
	m_enemies.clear();
	m_drops.clear();
}