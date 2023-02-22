//---------------------------------------------------------------------------------
// Collision.h
//---------------------------------------------------------------------------------
#ifndef _COLLISION_H
#define _COLLISION_H
//---------------------------------------------------------------------------------
#include <windows.h> 
//---------------------------------------------------------------------------------
// Following the rule of composition over inheritance,
// Certain objects in the game will have a Collider structure,
// which is used to define their hitboxes and to determine collisions
// between players, enemies, walls, items, etc.
//---------------------------------------------------------------------------------

static const int windowHeight = 768;
static const int windowWidth = 1025;

enum ColliderTag {
	OBSTACLE,
	ENEMY,
	PLAYER,
	BOUNDS,
	PROJECTILE,
	ITEM
};

struct Collider {
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	char tag;
};

bool GetMouseIsInBounds(Collider collider, float xPos, float yPos);
//---------------------------------------------------------------------------------
// Checks for collisions against a given collider.
// Returns a list of all colliders which are colliding with the given collider.
// I made this function to clean up my code, as most of my objects previously had 
// a DetectCollisions function which were largely written the exact same way.
//---------------------------------------------------------------------------------
std::vector<Collider> GetCollisions(Collider ownCollider, std::vector<Collider> otherColliders);
Collider CreateCollider(float x, float y, int xSize, int ySize, int tag, int xOffset = 0, int yOffset = 0);
void DrawCollider(Collider collider); // Draws a given collider (used for debugging)

#endif