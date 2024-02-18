#pragma once
#include "BuildingBase.h"
#include "TowerParts.h"

class Tower : public BuildingBase
{
	typedef BuildingBase base;

public:
	Tower();
	~Tower();

	//bool Init(int modelHandle, VECTOR startPos) override;
	bool Init(std::array<int, 3> modelHandle, VECTOR startPos);
	bool Process() override;
	bool Render() override;

	void SetFalling(VECTOR vDir);

	void UpdateCollision();

	// �f�o�b�O���̕\��
	bool DrawDebugInfo() override;

protected:

	// �p�[�c�̏��
	std::vector<TowerParts*> _towerParts;

	bool _use;
	int _partsNum;
	
	bool _isFalling;
	int _prevFallCnt;
	int _fallCnt;


	int _bottomIndex;

	VECTOR _startPos;
	VECTOR _endPos;

	float _r;
	float _up;
};