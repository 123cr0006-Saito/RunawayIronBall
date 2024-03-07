#pragma once
#include "appframe.h"
#include "BreakObject.h"

class BuildingBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	virtual bool Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	virtual bool Process();
	virtual bool Render();

	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

	VECTOR GetPos() { return _pos; }

	bool GetUseCollision() { return _useCollision; }
	void SetUseCollision(bool b) { _useCollision = b; }

	// �f�o�b�O���̕\��
	virtual bool DrawDebugInfo();

protected:
	// �����蔻����s�����ǂ���
	bool _useCollision;

	int _modelHandle;
	VECTOR _pos;

	BreakObject* _breakObj;
};