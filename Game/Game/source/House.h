#pragma once
#include "appframe.h"
#include "BuildingBase.h"
#include "BreakObject.h"


class House : public BuildingBase
{
public:
	House();
	~House();

	void Init(int modelHandle,std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength, int hp, int exp);
	void Process() override;

	void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) override;


	// �f�o�b�O���̕\��
	void DrawDebugInfo() override;

protected:
	// �j�󏈗��̗L����
	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

	int _hp;
	int _exp;
	// �j�󏈗��N���X
	BreakObject* _breakObj;
};