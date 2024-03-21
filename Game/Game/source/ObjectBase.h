#pragma once
#include "Cell.h"
#include "CollisionManager.h"
//class CharacterBase;
//class Player;
//class EnemyBase;

class CollisionManager;
class Cell;

class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	CollisionManager* _collisionManager;
	Cell* _cell;
};