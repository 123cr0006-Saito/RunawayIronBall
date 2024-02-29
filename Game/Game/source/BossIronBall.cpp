#include "BossIronBall.h"

namespace {
	constexpr int CHAIN_MAX = 10;
	constexpr float CHAIN_TOTAL_LENGTH = 1000.0f;
}

BossIronBall::BossIronBall()
{
	_ibModelHandle = -1;
	_ibPos = VGet(0.0f, 0.0f, 0.0f);
	_chainModelHandle = -1;
	_chainPos.clear();
	_chainDistance = 0.0f;
	_stakePos = nullptr;
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
}

void BossIronBall::Process()
{
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
