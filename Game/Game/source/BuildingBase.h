#pragma once
#include "appframe.h"
#include "ObjectBase.h"
#include "BreakObject.h"

class BuildingBase : public ObjectBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	virtual void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	virtual void Process();
	virtual void Render();

	VECTOR GetPos() { return _pos; }

	bool GetUseCollision() { return _useCollision; }
	void SetUseCollision(bool b) { _useCollision = b; }
	bool GetCanBreak() { return _canBreak; }
	OBB GetOBBCollision() { return obb; }


	void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) {};

	// �f�o�b�O���̕\��
	virtual void DrawDebugInfo();

protected:
	// ���f���n���h��
	int _modelHandle;
	// ���f���̍��W
	VECTOR _pos;

	// �����蔻����s�����ǂ���
	bool _useCollision;
	// �󂹂�I�u�W�F�N�g���ǂ���
	bool _canBreak;
	// �����蔻��
	OBB obb;
};