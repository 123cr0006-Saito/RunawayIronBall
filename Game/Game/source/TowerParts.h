//----------------------------------------------------------------------
// @filename TowerParts.h
// ��date: 2024/04/01
// ��author: Morozumi Hiroya
// @explanation
// �^���[�p�[�c�̐�����s���N���X
// 1�̃^���[�I�u�W�F�N�g�́A������TowerParts�N���X����\�������
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "ObjectBase.h"

class TowerParts : public ObjectBase
{
public:

	TowerParts();
	virtual ~TowerParts();

	// ����������
	void Init(int modelHandle, VECTOR startPos);
	// �X�V����
	void Process();
	// �`�揈��
	void Render();

	// �����蔻��̍X�V
	void UpdateCollision();



	void SetUse(bool use) { _use = use; }
	bool GetUse() { return _use; }

	int GetModelHandle() { return _modelHandle; }

	VECTOR GetPos() { return _pos; }

	void SetUseCollision(bool useCollision) {
		_useCollision = useCollision;
		if (_useCollision) {
			UpdateCollision();
		}
	}

	void SetBlast(VECTOR vDir) {
		SetUseCollision(true);
		_blast = true;
		_blastDir = vDir;

		_isFalling = false;
	}

	bool GetIsBlast() { return _blast; }

	void SetFalling(VECTOR endPos) {
		_isFalling = true;
		_fallCnt = 0;
		_fallStartPos = _pos;
		_fallEndPos = endPos;
	}

	bool GetIsFalling() { return _isFalling; }

	Sphere GetCollision() { return _sphereCollision; }


	// �f�o�b�O���̕\��
	void DrawDebugInfo();

protected:
	// ������я���
	void BlastOffProcess();
	// ��������
	void FallProcess();


protected:
	bool _use;

	int _modelHandle;
	VECTOR _pos;
	VECTOR _vRot;

	bool _useCollision;

	bool _blast;
	VECTOR _blastDir;
	int _blastCnt;

	bool _isFalling;
	int _fallCnt;
	VECTOR _fallStartPos;
	VECTOR _fallEndPos;




	Sphere _sphereCollision;
};