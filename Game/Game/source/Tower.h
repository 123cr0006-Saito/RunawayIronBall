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
	bool Init(std::array<int, 3> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	bool Process() override;
	bool Render() override;

	bool GetUse() { return _use; }

	void SetBlast(VECTOR vDir);
	bool GetCanBlast() { return _canBlast; }

	void UpdateCollision();


	Sphere GetBottomSphereCollision() { return _towerParts[_bottomIndex]->GetSphereCollision(); }


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

	bool _canBlast;

	// �ŉ����̃p�[�c�̃C���f�b�N�X
	int _bottomIndex;



	Sphere* _bottomSphereCollision;

};