//---------------------------------------------------------------------------------
// Collision.cpp
//---------------------------------------------------------------------------------
#include "stdafx.h"
#include <windows.h> 

#include "App/app.h"

#include "Collision.h"
//---------------------------------------------------------------------------------

Collider CreateCollider(float x, float y, int xSize, int ySize, int tag, int xOffset, int yOffset) {
	Collider newCollider;
	newCollider.xMin = x - (xSize / 2) + xOffset;
	newCollider.xMax = x + (xSize / 2) + xOffset;
	newCollider.yMin = y - (ySize / 2) + yOffset;
	newCollider.yMax = y + (ySize / 2) + yOffset;
	newCollider.tag = tag;
	return newCollider;
}

std::vector<Collider> GetCollisions(Collider ownCollider, std::vector<Collider> otherColliders) {
	std::vector<Collider> collisions;
	for (int i = 0; i < otherColliders.size(); i++) {
		if (ownCollider.xMax >= otherColliders[i].xMin && ownCollider.xMin <= otherColliders[i].xMax && ownCollider.yMax >= otherColliders[i].yMin && ownCollider.yMin <= otherColliders[i].yMax) {
			collisions.push_back(otherColliders[i]);
		}
	}
	return collisions;
}

bool GetMouseIsInBounds(Collider collider, float xPos, float yPos){
	return xPos > collider.xMin && xPos < collider.xMax && yPos > collider.yMin && yPos < collider.yMax;
}

void DrawCollider(Collider collider) {
	App::DrawLine(collider.xMin, collider.yMin, collider.xMax, collider.yMin);
	App::DrawLine(collider.xMin, collider.yMin, collider.xMin, collider.yMax);
	App::DrawLine(collider.xMax, collider.yMin, collider.xMax, collider.yMax);
	App::DrawLine(collider.xMin, collider.yMax, collider.xMax, collider.yMax);
}
