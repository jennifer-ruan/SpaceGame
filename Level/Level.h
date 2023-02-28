//---------------------------------------------------------------------------------
// Level.h
//---------------------------------------------------------------------------------
#ifndef _LEVEL_H
#define _LEVEL_H
//---------------------------------------------------------------------------------
#include <vector>
#include <ctime>
//---------------------------------------------------------------------------------
#include "app\app.h"
#include "Enemy.h"
#include "Item.h"
#include "Collision.h"
//---------------------------------------------------------------------------------

const int tileRows = 25;
const int tileCols = 33;
const int numGrassTiles = 256;
const int numWallTiles = 20;

enum tileType {
	GRASS,
	WALL
};

struct Tile {
	CSimpleSprite* tileSprite;
	bool collision;
};

class Level {
public:
	Level();
	std::vector<Collider> GetObstacles() { return m_obstacles; };
	std::vector<Collider> GetColliders();
	void Update(float dt, std::vector<Collider> colliders, float playerXPos, float playerYPos);
	void Draw();
	void Shutdown();
private:
	float m_rows = tileRows;
	float m_cols = tileCols;
	int m_wallSpawnChance = 0;
	float m_enemySpawnInterval = 2.0f;
	//int m_background = rand() % 4;
	time_t m_timeEnemyLastSpawned = time(0);
	Tile m_tiles[tileRows][tileCols];
	std::vector<Collider> m_obstacles;
	std::vector<Enemy*> m_enemies;
	std::vector<Item*> m_drops;
	Tile GetRandomTile(tileType type);
	//------------------------------------------------------------------------
	// Check if enemySpawnInterval amount of time has passed since the last enemy spawn.
	// If it has, spawn another one.
	//------------------------------------------------------------------------
	void SpawnEnemy();
};

#endif