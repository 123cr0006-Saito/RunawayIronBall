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
	void BlastOffProcess();
	void FallProcess();

	void Render();

	void UpdateCollision();



	void SetUse(bool use) {
		_use = use;
	}

	bool GetUse() {
		return _use;
	}

	void SetBlast(VECTOR vDir) {
		_blast = true;
		_blastDir = vDir;

		_isFalling = false;
	}

	void SetFalling(VECTOR endPos) {
		_isFalling = true;
		_fallCnt = 0;
		_fallStartPos = _pos;
		_fallEndPos = endPos;
	}

	bool GetIsFalling() {
		return _isFalling;
	}

protected:
	bool _use;

	bool _useCollision;

	bool _blast;
	VECTOR _blastDir;

	bool _isFalling;
	int _fallCnt;
	VECTOR _fallStartPos;
	VECTOR _fallEndPos;


	int _modelHandle;
	VECTOR _pos;
	VECTOR _vRot;

	Sphere _sphereCollision;

	VECTOR _localCenterPos;
};