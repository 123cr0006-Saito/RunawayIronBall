//----------------------------------------------------------------------
// @filename TowerParts.h
// @date: 2024/04/01
// @author: Morozumi Hiroya
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
	void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
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

	// �����蔻��̎g�p�ݒ�
	void SetUseCollision(bool useCollision) {
		_useCollision = useCollision;
		if (_useCollision) {
			UpdateCollision();
		}
	}

	// ������я����̐ݒ�
	void SetBlast(VECTOR vDir) {
		SetUseCollision(true);
		_blast = true;
		_blastDir = vDir;

		_isFalling = false;
	}

	bool GetIsBlast() { return _blast; }

	// ���������̐ݒ�
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

	// ���̃^���[�p�[�c���g�p����Ă��邩�ǂ���
	bool _use;

	// ���f���n���h��
	int _modelHandle;
	// ���W
	VECTOR _pos;

	bool _useCollision;

	//--------------------------------------------------
	// ������я���

	// ������ђ����ǂ���
	bool _blast;
	// ������ѕ���
	VECTOR _blastDir;
	// ������уJ�E���g
	int _blastCnt;
	//--------------------------------------------------

	//--------------------------------------------------
	// ��������

	// ���������ǂ���
	bool _isFalling;
	// �������̃J�E���g
	int _fallCnt;
	// �����J�n�ʒu
	VECTOR _fallStartPos;
	// �����I���ʒu
	VECTOR _fallEndPos;
	// -------------------------------------------------


	// �����蔻��`��
	// ������я�Ԃł́A�G�⌚���Ƃ̓����蔻��Ɏg�p����
	Sphere _sphereCollision;
};