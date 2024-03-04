#include "BossIronBall.h"

namespace {
	constexpr int CHAIN_MAX = 10;
	constexpr float CHAIN_TOTAL_LENGTH = 500.0f;


	constexpr float ROTAION_RADIUS_MAX = 1000.0f;

	// 1フレームあたりの回転角
	constexpr float ROTATION_ANGULAR_VELOCITY_MIN = (2.0f * DX_PI) / 120.0f; // 60フレームで一回転
	constexpr float ROTATION_ANGULAR_VELOCITY_MAX = (2.0f * DX_PI) / 60.0f; // 20フレームで一回転

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
}

BossIronBall::~BossIronBall()
{
	_stakePos = nullptr;
}

void BossIronBall::LoadModel()
{
	_ibModelHandle = ResourceServer::MV1LoadModel("Bossnake", "res/Enemy/Bossnake/Bossnake/cg_Boss.mv1");
	_chainModelHandle = ResourceServer::MV1LoadModel("Chain", "res/Chain/Cg_Chain.mv1");
}

void BossIronBall::Init(VECTOR* stakePos)
{
	_stakePos = stakePos;
	_chainDistance = CHAIN_TOTAL_LENGTH / CHAIN_MAX;
	for (int i = 0; i < CHAIN_MAX; i++) {
		_chainPos.push_back(VAdd(*_stakePos, VGet(0.0f, 0.0f, -_chainDistance * 2 * i)));
	}


	_ibPos = _chainPos[CHAIN_MAX - 1];
	_ibSphereCol.centerPos = _ibPos;
	_ibSphereCol.r = 20.0f * SCALE;
	MV1SetScale(_ibModelHandle, VScale(VGet(1.0f, 1.0f, 1.0f), SCALE));


	_mStakePos = MGetTranslate(*_stakePos);
	_mStakePosInv = MInverse(_mStakePos);


	_rotAngle = -1.0f;
}

void BossIronBall::Process()
{
	_activeRotationAcceleration = true;
	RotationAcceleration();
	RotationProcess();
	_ibPos = _chainPos[CHAIN_MAX - 1];
	_ibPos.y -= 1.0f;
	if (_ibPos.y - _ibSphereCol.r < 0.0f) _ibPos.y = _ibSphereCol.r;
	UpdateIBCollision();

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
	for (int i = 0; i < CHAIN_MAX; i++) {
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

void BossIronBall::DrawDebugInfo()
{
	_ibSphereCol.Render(COLOR_GREEN);

	//int x = 0;
	//int y = 500;
	//int line = 0;
	//DrawBox(x, y, x + 300, y + 20 * 5, GetColor(0, 0, 0), TRUE);
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "1周にかかるフレーム数:%3.2f", debugRotFrame); line++;
}

void BossIronBall::RotationProcess()
{
	static float rotDir = -1.0f;
	float angle = _rotAngle * rotDir;
	MATRIX mR = MGetRotY(angle);

	MATRIX m = MMult(_mStakePosInv, mR);
	m = MMult(m, _mStakePos);
	_chainPos[CHAIN_MAX - 1] = VTransform(VGet(0.0f, 0.0f, -ROTAION_RADIUS_MAX), m);




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
