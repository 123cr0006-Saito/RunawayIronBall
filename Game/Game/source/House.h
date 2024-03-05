#pragma once
#include "appframe.h"
#include "BreakObject.h"


class House
{
public:
	House();
	~House();

	void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	void Process();
	void Render();

	VECTOR GetPos() { return _pos; }

	bool GetUseCollision() { return _useCollision; }
	void SetUseCollision(bool b) { _useCollision = b; }
	OBB GetOBBCollision() { return obb; }

	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));




	// �f�o�b�O���̕\��
	void DrawDebugInfo();

protected:
	// ���f���n���h��
	int _modelHandle;
	// ���f���̍��W
	VECTOR _pos;

	// �����蔻����s�����ǂ���
	bool _useCollision;
	// �����蔻��
	OBB obb;

	// �j�󏈗��N���X
	BreakObject* _breakObj;
};