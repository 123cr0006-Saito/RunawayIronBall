//----------------------------------------------------------------------
// @filename BuildingBase.h
// ��date: 2024/04/01
// ��author: Morozumi Hiroya
// @explanation
// �����N���X�̊��N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "ObjectBase.h"
#include "BreakObject.h"

class BuildingBase : public ObjectBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	// ����������
	virtual void Init(int modelHandle, std::string objName,VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	// �X�V����
	virtual void Process();
	// �`�揈��
	virtual void Render();

	std::string GetName() { return _objName; }
	virtual int GetHp() { return 0; }
	virtual int GetExp() { return 0; }
	int GetSuppression() { return _suppression; }

	VECTOR GetPos() { return _pos; }

	bool GetCanBreak() { return _canBreak; }

	// �����蔻��̎g�p�ݒ�
	void SetUseCollision(bool b);

	bool GetUseCollision() { return _useCollision; }
	OBB GetOBBCollision() { return _obbCollision; }

	// ��_���[�W�ݒ�
	// House�N���X�ŃI�[�o�[���C�h
	virtual void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) {};

	// �f�o�b�O���̕\��
	virtual void DrawDebugInfo();

protected:
	// ���f���n���h��
	int _modelHandle;
	std::string _objName;
	int _hp; // HP
	int _exp; // �o���l
	int _suppression; // �����l
	// ���f���̍��W
	VECTOR _pos;

	// �󂹂�I�u�W�F�N�g���ǂ���
	// �p����Őݒ�
	bool _canBreak;

	// �����蔻����s�����ǂ���
	bool _useCollision;
	// �����蔻��
	OBB _obbCollision;
};