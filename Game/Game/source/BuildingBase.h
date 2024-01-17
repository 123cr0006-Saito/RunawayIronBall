#pragma once
#include "appframe.h"
#include "BreakObject.h"

class BuildingBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	virtual bool Init(int modelHandl, VECTOR startPos);
	virtual bool Process();
	virtual bool Render();

	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

private:
	int _modelHandle;
	VECTOR _pos;

	BreakObject* _breakObj;





	// �f�o�b�O�p
	// �p�[�c�̐�����я������s���܂ł̃t���[����
	int _testCnt;
};