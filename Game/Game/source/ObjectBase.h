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
	void SetModelName(std::string name, std::string path) { _name = name; _path = path; };
	void LoadModel();

	int _modelHandle;
	std::string _path , _name;
	CollisionManager* _collisionManager;
	Cell* _cell;
};