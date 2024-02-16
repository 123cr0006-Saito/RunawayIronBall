#pragma once
#include "appframe.h"

class TowerParts
{
public:
	friend class Tower;

	TowerParts();
	~TowerParts();


	void Init(int modelHandle, VECTOR startPos);
	void Process();
	void Render();

	void UpdateCollision();

protected:
	bool _use;

	bool _useCollision;

	bool _blast;
	VECTOR _blastDir;


	int _modelHandle;
	VECTOR _pos;
	VECTOR _vRot;

	Sphere _sphereCollision;

	VECTOR _localCenterPos;
};