#include "Boss.h"

Boss::Boss()
{
	_stakeModelHandle = -1;
	_stakePos = VGet(0.0f, 0.0f, 0.0f);
	
	_stakeCapsuleCol.down_pos = VGet(0.0f, 0.0f, 0.0f);
	_stakeCapsuleCol.up_pos = VGet(0.0f, 0.0f, 0.0f);
	_stakeCapsuleCol.r = 0.0f;
	_stakeCapsuleCol.up = 0.0f;
}

Boss::~Boss()
{
}

void Boss::LoadModel()
{
	_stakeModelHandle = MV1LoadModel("res/Enemy/Bossnake/Stake/test_Tower_01.mv1");
}

void Boss::Init(VECTOR polePos)
{
	_stakePos = polePos;
	MV1SetPosition(_stakeModelHandle, _stakePos);

	_stakeCapsuleCol.down_pos = _stakePos;
	_stakeCapsuleCol.up = 500.0f;
	_stakeCapsuleCol.up_pos = VAdd(_stakePos, VGet(0.0f, _stakeCapsuleCol.up, 0.0f));
	_stakeCapsuleCol.r = 100.0f;
}

void Boss::Process()
{
	
}

void Boss::Render()
{
	MV1DrawModel(_stakeModelHandle);
}

void Boss::DrawDebugInfo()
{
	_stakeCapsuleCol.Render(COLOR_GREEN);
}
