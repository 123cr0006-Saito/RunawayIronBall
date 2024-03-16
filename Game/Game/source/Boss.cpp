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
	ResourceServer::MV1DeleteModel("Stake", _stakeModelHandle);
	_stakeModelHandle = -1;

	delete _ironBall;
	_ironBall = nullptr;
}

void Boss::LoadModel()
{
	_stakeModelHandle = ResourceServer::MV1LoadModel("Stake", "res/Enemy/Cg_Enemy_Bossnake/CG_OBJ_Stake.mv1");
	_ironBall->LoadModel();
}

void Boss::Init(VECTOR polePos)
{
	_stakePos = polePos;
	MV1SetPosition(_stakeModelHandle, VAdd(_stakePos, VGet(0.0f, -200.0f, 0.0f)));

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
