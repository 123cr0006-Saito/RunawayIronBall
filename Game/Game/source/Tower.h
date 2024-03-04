#pragma once
#include "TowerParts.h"

class Tower
{
public:
	Tower();
	~Tower();


	void Init(std::array<int, 3> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	void Process();
	void Render();

	bool GetUse() { return _use; }

	VECTOR GetPos() { return _pos; }

	void SetBlast(VECTOR vDir);
	bool GetCanBlast() { return _canBlast; }

	void UpdateCollision();


	Sphere GetBottomSphereCollision() { return _towerParts[_bottomIndex]->GetSphereCollision(); }


	// �f�o�b�O���̕\��
	void DrawDebugInfo();

protected:
	// �^���[�p�[�c���c���Ă��邩�ǂ���
	bool _use;
	// �N�_���W
	VECTOR _pos;

	// �p�[�c�̏��
	std::vector<TowerParts*> _towerParts;

	// �p�[�c�̏����̐�
	int _partsNum;
	
	bool _isFalling;
	int _prevFallCnt;
	int _fallCnt;

	bool _canBlast;

	// �ŉ����̃p�[�c�̃C���f�b�N�X
	int _bottomIndex;


	Sphere* _bottomSphereCollision;

};