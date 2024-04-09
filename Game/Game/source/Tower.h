//----------------------------------------------------------------------
// @filename Tower.cpp
// ��date: 2024/04/01
// ��author: Morozumi Hiroya
// @explanation
// �^���[�I�u�W�F�N�g�̐���E�`����s���N���X
// 1�̃^���[�I�u�W�F�N�g�́A������TowerParts�N���X����\�������
//----------------------------------------------------------------------
#pragma once
#include "TowerParts.h"
#include "ObjectBase.h"

class Tower : public ObjectBase
{
public:
	Tower();
	virtual ~Tower();


	void Init(std::vector<int> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	void Process();
	void Render();

	bool GetUse() { return _use; }

	VECTOR GetPos() { return _pos; }

	void SetBlast(VECTOR vDir);
	bool GetCanBlast() { return _canBlast; }


	Sphere GetCollision() { return _sphereCollision; }


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


	Sphere _sphereCollision;

};