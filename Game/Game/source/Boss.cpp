#include "Boss.h"

Boss::Boss()
{
	_stakeModelHandle = -1;
	_stakePos = VGet(0.0f, 0.0f, 0.0f);
	
	_stakeCapsuleCol.down_pos = VGet(0.0f, 0.0f, 0.0f);
	_stakeCapsuleCol.up_pos = VGet(0.0f, 0.0f, 0.0f);
	_stakeCapsuleCol.r = 0.0f;
	_stakeCapsuleCol.up = 0.0f;

	_ironBall = new BossIronBall();
}

Boss::~Boss()
{
}

void Boss::LoadModel()
{
	_stakeModelHandle = MV1LoadModel("res/Enemy/Bossnake/Stake/test_Tower_01.mv1");
	_ironBall->LoadModel();
}

void Boss::Init(VECTOR polePos)
{
	_stakePos = polePos;
	MV1SetPosition(_stakeModelHandle, _stakePos);

	_stakeCapsuleCol.down_pos = _stakePos;
	_stakeCapsuleCol.up = 500.0f;
	_stakeCapsuleCol.up_pos = VAdd(_stakePos, VGet(0.0f, _stakeCapsuleCol.up, 0.0f));
	_stakeCapsuleCol.r = 100.0f;

	_ironBall->Init(&_stakePos);
}

void Boss::Process()
{
	_ironBall->Process();
}

void Boss::Render()
{
	MV1DrawModel(_stakeModelHandle);
	_ironBall->Render();
}

void Boss::DrawDebugInfo()
{
	_stakeCapsuleCol.Render(COLOR_GREEN);

	_ironBall->DrawDebugInfo();
}
