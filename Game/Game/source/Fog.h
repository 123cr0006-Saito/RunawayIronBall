//----------------------------------------------------------------------
// @filename Fog.h
// ��date: 2024/02/22
// ��author: saito ko
// @explanation
// �v���C���[�����߂�ꂽ�͈͊O�ɏo��ƃt�H�O����������N���X
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include "player.h"

class Fog
{
	public:
	Fog();
	~Fog();
	void Process(int stageNum);
	void UpdateIsFog(int stageNum);
	bool GetIsFog() { return IsFog; }
protected:
	Player* _player;
	bool IsFog;
	const int _fogDistanceMax = 30000;
	const int _fogDistanceMin = 2000;
	float _fogEndDistance;
	int _param;
	int _easingCount;
};

