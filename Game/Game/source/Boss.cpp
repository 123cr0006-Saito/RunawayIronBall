#include "Boss.h"

namespace
{
	constexpr float SEARCH_RANGE[2] = { 2000.0f, 3000.0f };

}
Boss::Boss()
{
	_stakeModelHandle = -1;
	_stakePos = VGet(0.0f, 0.0f, 0.0f);
	
	_stakeCapsuleCol.down_pos = VGet(0.0f, 0.0f, 0.0f);
	_stakeCapsuleCol.up_pos = VGet(0.0f, 0.0f, 0.0f);
	_stakeCapsuleCol.r = 0.0f;
	_stakeCapsuleCol.up = 0.0f;

	_ironBall = NEW BossIronBall();

	_player = nullptr;
}

Boss::~Boss()
{
	ResourceServer::MV1DeleteModel("Stake", _stakeModelHandle);
	_stakeModelHandle = -1;

	delete _ironBall;
	_ironBall = nullptr;

	_player = nullptr;
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

	_searchRange[0] = 2000.0f;
	_searchRange[1] = 3000.0f;

	_ironBall->Init(&_stakePos);

	_player = Player::GetInstance();
}

void Boss::Process()
{
	_ironBall->Process();
	CheckHitBossAndStake();
}

void Boss::Render()
{
	MV1DrawModel(_stakeModelHandle);
	_ironBall->Render();
}

int Boss::SearchPlayer()
{
	int rangeIndex = -1;
	VECTOR playerPos = _player->GetPosition();
	VECTOR vSub = VSub(playerPos, _stakePos);

	float squareLength = VSquareSize(vSub);
	if (squareLength < _searchRange[0] * _searchRange[0])
	{
		rangeIndex = 0;
	}
	else if (squareLength < _searchRange[1] * _searchRange[1])
	{
		rangeIndex = 1;
	}

	return rangeIndex;
}

void Boss::CheckHitBossAndStake()
{
	_ironBall->UpdateIBCollision();
	Sphere ibCol = _ironBall->GetIBCollision();
	VECTOR shortestPos = VGet(0.0f, 0.0f, 0.0f);
	
	if (Collision3D::SphereCapsuleCol(ibCol, _stakeCapsuleCol, &shortestPos))
	{
		VECTOR vDir = VSub(ibCol.centerPos, shortestPos);
		vDir = VNorm(vDir);
		float length = _stakeCapsuleCol.r + ibCol.r + 20.0f;
		VECTOR vMove = VAdd(shortestPos, VScale(vDir, length));
		_ironBall->SetPosition(vMove);
		_ironBall->SetHitStake(true);
	}
}

void Boss::DrawDebugInfo()
{
	_stakeCapsuleCol.Render(COLOR_GREEN);

	for (int i = 0; i < 2; i++) {
		Sphere searchRange = { _stakePos, _searchRange[i]};
		searchRange.Render(COLOR_RED);
	}

	_ironBall->DrawDebugInfo();
}
