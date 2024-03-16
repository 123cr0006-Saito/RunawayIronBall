#include "BossIronBall.h"

namespace {
	constexpr int BOSS_CHAIN_MAX = 10;
	constexpr float BOSS_CHAIN_TOTAL_LENGTH = 500.0f;


	constexpr float ROTAION_RADIUS_MAX = 1000.0f;

	// 待機状態
	// 小ジャンプの間隔フレーム数（120 ~ 180）
	// 基準値 
	constexpr int IDLE_INTERVAL_BASE = 120;
	// 乱数で追加されるフレーム数の最大値
	constexpr int IDLE_INTERVAL_ADD_MAX = 60;

	constexpr int IDLE_CNT_MAX = 30;
	constexpr float IDLE_MOVE_SPEED = 180.0f / IDLE_CNT_MAX; // 30フレームで1.8m移動

	// 落下攻撃
	// 飛び上がってから、最高地点に到達するまでのフレーム数
	constexpr int DR_REACH_HIGHEST_CNT = 90;
	// 最高地点に到達してから、地面に着地するまでのフレーム数
	constexpr int DR_REACH_GROUND_CNT = 30;
	// 地面に着地後の硬直時間
	constexpr int DR_STIFFEN_CNT = 60;

	// 1フレームあたりの回転角
	constexpr float ROTATION_ANGULAR_VELOCITY_MIN = (2.0f * DX_PI) / 90.0f; // 60フレームで一回転
	constexpr float ROTATION_ANGULAR_VELOCITY_MAX = (2.0f * DX_PI) / 20.0f; // 20フレームで一回転

	constexpr int ROTATION_ACCELERATION_CNT_MAX = 180;

	// 仮
	constexpr float SCALE = 12.8f;
}

BossIronBall::BossIronBall()
{
	_ibModelHandle = -1;
	_ibPos = VGet(0.0f, 0.0f, 0.0f);
	_ibSphereCol.centerPos = _ibPos;
	_ibSphereCol.r = 0.0f;
	_ibState = IB_STATE::IB_STATE_IDLE;

	_phase = 0;
	_phaseCnt = 0;

	_posBeforeMoving = VGet(0.0f, 0.0f, 0.0f);
	_targetPos = VGet(0.0f, 0.0f, 0.0f);

	_ibIdleCnt = 0;
	_ibMoveDir = VGet(0.0f, 0.0f, 0.0f);


	_ibStiffenCnt = 0;


	_activeRotationAcceleration = false;
	_rotationAccelerationCnt = 0;
	_rotAngularVelocity = 0.0f;
	_rotAngle = 0.0f;

	_chainModelHandle = -1;
	_chainPos.clear();
	_chainDistance = 0.0f;

	_stakePos = nullptr;
	_mStakePos = MGetIdent();
	_mStakePosInv = MGetIdent();


	_player = nullptr;
}

BossIronBall::~BossIronBall()
{
	ResourceServer::MV1DeleteModel("Bossnake", _ibModelHandle);
	_ibModelHandle = -1;
	ResourceServer::MV1DeleteModel("Chain", _chainModelHandle);
	_chainModelHandle = -1;

	_stakePos = nullptr;

	_player = nullptr;
}

void BossIronBall::LoadModel()
{
	_ibModelHandle = ResourceServer::MV1LoadModel("Bossnake", "res/Enemy/Cg_Enemy_Bossnake/Cg_Enemy_Bossnake.mv1");
	_chainModelHandle = ResourceServer::MV1LoadModel("Chain", "res/Chain/Cg_Chain.mv1");
}

void BossIronBall::Init(VECTOR* stakePos)
{
	_stakePos = stakePos;
	_chainDistance = BOSS_CHAIN_TOTAL_LENGTH / BOSS_CHAIN_MAX;
	for (int i = 0; i < BOSS_CHAIN_MAX; i++) {
		_chainPos.push_back(VAdd(*_stakePos, VGet(0.0f, 0.0f, -_chainDistance * 2 * i)));
	}


	_ibPos = _chainPos[BOSS_CHAIN_MAX - 1];
	_ibSphereCol.centerPos = _ibPos;
	_ibSphereCol.r = 20.0f * SCALE;
	MV1SetScale(_ibModelHandle, VScale(VGet(1.0f, 1.0f, 1.0f), SCALE));

	_ibState = IB_STATE::IB_STATE_IDLE;


	_mStakePos = MGetTranslate(*_stakePos);
	_mStakePosInv = MInverse(_mStakePos);


	_rotAngle = -1.0f;


	_player = Player::GetInstance();
}

void BossIronBall::Process()
{
	switch (_ibState)
	{
	case BossIronBall::IB_STATE::IB_STATE_IDLE:
		IdleProcess();
		break;
	case BossIronBall::IB_STATE::IB_STATE_STIFFEN:
		StiffenProcess();
		break;
	case BossIronBall::IB_STATE::IB_STATE_ATTACK_RUSH:
		break;
	case BossIronBall::IB_STATE::IB_STATE_ATTACK_DROP:
		DropProcess();
		break;
	case BossIronBall::IB_STATE::IB_STATE_ATTACK_ROTATION:
		_activeRotationAcceleration = true;
		RotationAcceleration();
		RotationProcess();
		break;
	}

	//_ibPos = _chainPos[CHAIN_MAX - 1];
	//_ibPos.y -= 1.0f;
	if (_ibPos.y - _ibSphereCol.r < 0.0f) _ibPos.y = _ibSphereCol.r;
	UpdateIBCollision();


	// 仮
	if (1 == CheckHitKey(KEY_INPUT_UP)) {
		debugFrame += 1;
	}
	else if (1 == CheckHitKey(KEY_INPUT_DOWN)) {
		debugFrame -= 1;
		if(debugFrame < 1) debugFrame = 1;
	}

	if (1 == CheckHitKey(KEY_INPUT_RIGHT)) {
		debugValue += 1.0f;
	}
	else if (1 == CheckHitKey(KEY_INPUT_LEFT)) {
		debugValue -= 1.0f;
	}

	//static float angle = 0.0f;
	//angle += 0.05f;
	//MATRIX mStake = MGetTranslate(*_stakePos);

	////MATRIX m = MInverse(mStake);
	////m = MMult(m, MGetRotY(angle));
	////m = MMult(m, mStake);
	////_chainPos[CHAIN_MAX - 1] = VTransform(VGet(0.0f, 0.0f, -1000.0f), m);

	//VECTOR v = VGet(0.0f, 0.0f, -1000.0f);
	//MATRIX m =  MGetRotY(angle);
	//m = MMult(m, mStake);
	//_chainPos[CHAIN_MAX - 1] = VTransform(v, m);


	//for (int i = 0; i < CHAIN_MAX - 1; i++) {
	//	VECTOR vNext = _chainPos[i + 1];
	//	VECTOR vDelta = VSub(vNext, _chainPos[i]);
	//	float distance = VSize(vDelta);
	//	float difference = _chainDistance - distance;

	//	float offsetX = (difference * vDelta.x / distance) * 0.9f;
	//	float offsetY = (difference * vDelta.y / distance) * 0.9f;
	//	float offsetZ = (difference * vDelta.z / distance) * 0.9f;

	//	if (i != 0) {
	//		_chainPos[i].x -= offsetX;
	//		_chainPos[i].y -= offsetY;
	//		_chainPos[i].z -= offsetZ;
	//	}
	//	float mul = 1.0f;
	//	//if (i == 0) mul = 2.0f;
	//	_chainPos[i + 1].x += offsetX * mul;
	//	_chainPos[i + 1].y += offsetY * mul;
	//	_chainPos[i + 1].z += offsetZ * mul;
	//}
	//_ibPos = _chainPos[CHAIN_MAX - 1];
}

void BossIronBall::Render()
{
	for (int i = 0; i < BOSS_CHAIN_MAX; i++) {
		MV1SetPosition(_chainModelHandle, _chainPos[i]);
		MV1DrawModel(_chainModelHandle);
	}

	MV1SetPosition(_ibModelHandle, _ibPos);
	MV1DrawModel(_ibModelHandle);
}

void BossIronBall::UpdateIBCollision()
{
	_ibSphereCol.centerPos = _ibPos;
}

void BossIronBall::CheckState()
{
	// 仮
	_ibState = IB_STATE::IB_STATE_ATTACK_DROP;

	switch (_ibState)
	{
	case BossIronBall::IB_STATE::IB_STATE_IDLE:
	{
		_ibIdleCnt = 0;

		// -100 ~ 100の範囲でランダムに移動方向を決定
		float x = (rand() % 201) - 100;
		float z = (rand() % 201) - 100;

		_ibMoveDir = VGet(x, 0.0f, z);
		_ibMoveDir = VNorm(_ibMoveDir);



		debugFrameMax = debugFrame;
		debugValueMax = debugValue;

		break;
	}
	case BossIronBall::IB_STATE::IB_STATE_STIFFEN:
		break;
	case BossIronBall::IB_STATE::IB_STATE_ATTACK_RUSH:
		break;
	case BossIronBall::IB_STATE::IB_STATE_ATTACK_DROP:
		SetDrop();
		break;
	case BossIronBall::IB_STATE::IB_STATE_ATTACK_ROTATION:
		break;
	}
}

void BossIronBall::ResetPhase()
{
	_phase = 0;
	_phaseCnt = 0;
}

void BossIronBall::IdleProcess()
{
	_ibPos = VAdd(_ibPos, VScale(_ibMoveDir, IDLE_MOVE_SPEED));

	float rad = _ibIdleCnt / 30.0f * DX_PI;
	_ibPos.y = sinf(rad) * 100.0f + _ibSphereCol.r;

	_ibIdleCnt++;
	if (_ibIdleCnt > IDLE_CNT_MAX) {
		_ibIdleCnt = IDLE_CNT_MAX;
		// 硬直状態に遷移
		_ibState = IB_STATE::IB_STATE_STIFFEN;
		SetStiffen(IDLE_INTERVAL_BASE + (rand() % IDLE_INTERVAL_ADD_MAX));
	}
}

void BossIronBall::StiffenProcess()
{
	_ibStiffenCnt--;
	if (_ibStiffenCnt < 0) {
		_ibStiffenCnt = 0;
		CheckState();
	}
}

void BossIronBall::DropProcess()
{
	switch (_phase)
	{
	case 0:				// 飛び上がり
	{
		VECTOR highestPos = VAdd(_targetPos, VGet(0.0f, 500.0f, 0.0f));
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::OutExpo(_phaseCnt, _posBeforeMoving.x, highestPos.x, DR_REACH_HIGHEST_CNT);
		v.y = Easing::OutExpo(_phaseCnt, _posBeforeMoving.y, highestPos.y, DR_REACH_HIGHEST_CNT);
		v.z = Easing::OutExpo(_phaseCnt, _posBeforeMoving.z, highestPos.z, DR_REACH_HIGHEST_CNT);
		_ibPos = v;
		_phaseCnt++;
		if (_phaseCnt > DR_REACH_HIGHEST_CNT) {
			_posBeforeMoving = _ibPos;
			_phaseCnt = 0;
			_phase++;
		}
	}
	break;

	case 1:				// 着地
	{
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.x, _targetPos.x, DR_REACH_GROUND_CNT);
		v.y = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.y, _targetPos.y, DR_REACH_GROUND_CNT);
		v.z = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.z, _targetPos.z, DR_REACH_GROUND_CNT);


		_ibPos = v;
		_phaseCnt++;
		if (_phaseCnt > DR_REACH_GROUND_CNT) {
			ResetPhase();
			SetStiffen(DR_STIFFEN_CNT);
		}
	}
	break;
	}
}

void BossIronBall::SetDrop()
{
	ResetPhase();

	_posBeforeMoving = _ibPos;
	_targetPos = _player->GetPosition();
	_targetPos.y = _ibSphereCol.r;
}

void BossIronBall::RotationProcess()
{
	static float rotDir = -1.0f;
	float angle = _rotAngle * rotDir;
	MATRIX mR = MGetRotY(angle);

	MATRIX m = MMult(_mStakePosInv, mR);
	m = MMult(m, _mStakePos);
	_ibPos = VTransform(VGet(0.0f, 0.0f, -ROTAION_RADIUS_MAX), m);




	MV1SetRotationXYZ(_ibModelHandle, VGet(0.0f, angle, 0.0f));



	_rotAngle += _rotAngularVelocity;
}

void BossIronBall::RotationAcceleration()
{
	if (_activeRotationAcceleration) {
		_rotAngularVelocity = Easing::Linear(_rotationAccelerationCnt, ROTATION_ANGULAR_VELOCITY_MIN, ROTATION_ANGULAR_VELOCITY_MAX, ROTATION_ACCELERATION_CNT_MAX);


		//_rotAngularVelocity = 2.0f * DX_PI / debugRotFrame;
		//
		//if (1 == CheckHitKey(KEY_INPUT_UP)) {
		//	debugRotFrame += 0.1f;
		//}
		//else if (1 == CheckHitKey(KEY_INPUT_DOWN)) {
		//	debugRotFrame -= 0.1f;
		//}


		_rotationAccelerationCnt++;
		if (_rotationAccelerationCnt > ROTATION_ACCELERATION_CNT_MAX) {
			_rotationAccelerationCnt = ROTATION_ACCELERATION_CNT_MAX;
		}
	}
}

void BossIronBall::DrawDebugInfo()
{
	_ibSphereCol.Render(COLOR_GREEN);

	int x = 0;
	int y = 500;
	int line = 0;
	DrawBox(x, y, x + 300, y + 500, GetColor(0, 0, 0), TRUE);
	std::array<std::string, 5> stateStr = { "IDLE", "STIFFEN", "ATTACK_RUSH", "ATTACK_DROP", "ATTACK_ROTATION" };
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "State:%s", stateStr[static_cast<int>(_ibState)].c_str()); line++;
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "pos: x %3.2f, y %3.2f, z %3.2f", _ibPos.x, _ibPos.y, _ibPos.z); line++;
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "_phase: %d, _phaseCnt: %d", _phase, _phaseCnt); line++;

	//line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "idleCnt:%d", _ibIdleCnt); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "idleCntMax:%d", debugFrameMax); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "次に設定するフレーム数:%d", debugFrame); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "次に設定する移動距離:%3.2f cm", debugValue); line++;
}