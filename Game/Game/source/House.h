#pragma once
#include "appframe.h"
#include "BuildingBase.h"
#include "BreakObject.h"


class House : public BuildingBase
{
public:
	House();
	~House();

	void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength) override;
	void Process() override;
	void Render() override;

	void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) override;


	// �f�o�b�O���̕\��
	void DrawDebugInfo();

protected:
	// �j�󏈗��̗L����
	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

	// �j�󏈗��N���X
	BreakObject* _breakObj;
};