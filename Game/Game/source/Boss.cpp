#include "Boss.h"

namespace {
	// 最大無敵時間
	constexpr int STAKE_INVINCIBLE_CNT_MAX = 60;
}

Boss::Boss()
{
	_stakeModelHandle = -1;
	_stakePos = VGet(0.0f, 0.0f, 0.0f);
	
	_stakeCapsuleCol.down_pos = VGet(0.0f, 0.0f, 0.0f);
	_stakeCapsuleCol.up_pos = VGet(0.0f, 0.0f, 0.0f);
	_stakeCapsuleCol.r = 0.0f;
	_stakeCapsuleCol.up = 0.0f;

	_stakeHp = 0;
	_isStakeInvincible = false;
	_stakeInvincibleCnt = 0;
	_isStakeBroken = false;

	_ironBall = NEW BossIronBall();

	_player = nullptr;
}

Boss::~Boss()
{
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

	_stakeHp = 100;

	_ironBall->Init(&_stakePos);

	_player = Player::GetInstance();
}

void Boss::Process()
{
	_ironBall->Process();
	if (!_isStakeBroken) {
		CheckHitBossAndStake();

		// HPが半分以下になったら鉄球を強化状態にする
		if (_stakeHp / static_cast<float>(STAKE_MAX_HP) <= 0.5f) {
			_ironBall->SetEnhanced();
		}

		// 杭の無敵時間を更新する
		if (_isStakeInvincible) {
			_stakeInvincibleCnt--;
			if (_stakeInvincibleCnt <= 0) {
				_isStakeInvincible = false;
			}
		}
	}
}

void Boss::Render()
{
	if (!_isStakeBroken) {
		MV1DrawModel(_stakeModelHandle);
	}
	_ironBall->Render();
}

void Boss::CheckHitBossAndStake()
{
	if(_ironBall->GetUseCollision() == false) { return; }
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

		// 鉄球がノックバック時ならはじき返し処理を行う
		if (_ironBall->CheckKnockBack()) {
			// ハードノックバック時にはBossIronBallクラス内で別の処理を行うため、ここでははじき返し処理を行わない
			if (_ironBall->CheckHardKnockBack() == false) {
				VECTOR vDir = VSub(ibCol.centerPos, _stakePos);
				vDir.y = 0.0f;
				_ironBall->SetKnockBack(vDir, 30.0f);
			}
			// 杭にダメージを与える
			SetDamageStake(20);
			global._soundServer->DirectPlay("House_Iron_Hit");
		}
	}
}

void Boss::SetDamageStake(int damage)
{
	if (!_isStakeInvincible) {
		_isStakeInvincible = true;
		_stakeInvincibleCnt = STAKE_INVINCIBLE_CNT_MAX;
		_stakeHp -= damage;
		if (_stakeHp <= 0) {
			_stakeHp = 0;
			_isStakeBroken = true;
			_ironBall->SetISStakeBroken(true);
			_ironBall->ChangeGlass();
			global._soundServer->DirectPlay("House_Iron_Break");
		}
	}
}

void Boss::DrawDebugInfo()
{
	if (!_isStakeBroken) {
		_stakeCapsuleCol.Render(COLOR_GREEN);
	}
	_ironBall->DrawDebugInfo();
}
