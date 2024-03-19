#include "IronBall.h"

namespace {
	constexpr float IB_BODY_COLLISION_RADIUS = 50.0f;
	constexpr float IB_ATTACK_COLLISION_RADIUS = 100.0f;
	constexpr float CHAIN_COLLISION_RADIUS = 75.0f;
}

IronBall::IronBall()
{
	_cModelHandle = -1;
	_iModelHandle = -1;
	_playerModelHandle = -1;
	_input = nullptr;
	_iForwardDir = VGet(0, 0, 0);

	for(int i = 0; i < CHAIN_MAX; i++) {
		_cPos[i] = VGet(0, 0, 0);
	}
	for(int i = 0; i < CHAIN_MAX; i++) {
		_m[i] = MGetIdent();
	}

	_iPos = VGet(0, 0, 0);
	_ibDefaultScale = VGet(0, 0, 0);

	_ibBodySphereCollision.centerPos = VGet(0, 0, 0);
	_ibBodySphereCollision.r = 0;
	_ibAttackSphereCollision.centerPos = VGet(0, 0, 0);
	_ibAttackSphereCollision.r = 0;

	_chainCapsuleCollision.up_pos = VGet(0, 0, 0);
	_chainCapsuleCollision.down_pos = VGet(0, 0, 0);
	_chainCapsuleCollision.r = 0;

	_chainCell = nullptr;

	_animIndex = 0;
	_animTotalTime = 0;
	_playTime = 0;
	_attackAnimCnt = 0;
	_cnt = 0;
	_attackDir = 1;
	_length = 0;
	_followingMode = false;
	_moveState = IB_MOVE_STATE::FOLLOWING;
	_enabledAttackCollision = false;
	_socketNo[0] = -1;
	_socketNo[1] = -1;
	_socketNo[2] = -1;
	_parent = nullptr;
	_parentPos = nullptr;
}

IronBall::~IronBall()
{
	_input = nullptr;
	_parent = nullptr;
	_parentPos = nullptr;
}

void IronBall::Init() {
	_input = XInput::GetInstance();

	_cModelHandle = MV1LoadModel("res/Chain/Cg_Chain.mv1");
	_cPos[0] = VGet(0.0f, 0.0f, 0.0f);
	MV1SetPosition(_cModelHandle, _cPos[0]);
	MV1SetScale(_cModelHandle, VGet(0.5f, 0.5f, 0.5f));

	for (int i = 1; i < CHAIN_MAX; i++) {
		_cPos[i] = VAdd(_cPos[i - 1], VGet(0.0f, 0.0f, -100.0f));
	}

	_iModelHandle = MV1LoadModel("res/Character/Cg_Iron_Ball/Cg_Iron_Ball.mv1");
	_iPos = VAdd(_cPos[CHAIN_MAX - 1], VGet(0.0f, 10.0f, 0.0f));
	_ibDefaultScale = VGet(2.5f, 2.5f, 2.5f);
	MV1SetScale(_iModelHandle, _ibDefaultScale);
	MV1SetPosition(_iModelHandle, _iPos);

	_ibBodySphereCollision.centerPos = _iPos;
	_ibBodySphereCollision.r = IB_BODY_COLLISION_RADIUS;
	_ibAttackSphereCollision.centerPos = _iPos;
	_ibAttackSphereCollision.r = IB_ATTACK_COLLISION_RADIUS;

	_chainCapsuleCollision.up_pos = _cPos[0];
	_chainCapsuleCollision.down_pos = _cPos[CHAIN_MAX - 1];
	_chainCapsuleCollision.r = CHAIN_COLLISION_RADIUS;


	_animIndex = MV1AttachAnim(_iModelHandle, 0);
	_animTotalTime = MV1GetAnimTotalTime(_iModelHandle, _animIndex);
	_playTime = 0.0f;


	_iForwardDir = VGet(0, 0, 0);


	_attackAnimCnt = 0;

	_cnt = 0.0f;
	_attackDir = 1;

	for (int i = 0; i < CHAIN_MAX; i++) {
		_m[i] = MGetIdent();
	}

	_length = 50.0f;

	_followingMode = false;
	_moveState = IB_MOVE_STATE::FOLLOWING;

	_enabledAttackCollision = false;

	_cell->_objType = OBJ_TYPE::PL_IB;
	
	_chainCell = NEW Cell();
	_chainCell->_obj = this;
	_chainCell->_objType = OBJ_TYPE::PL_IB_CHAIN;
}


void IronBall::Process() {
	MATRIX m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, _socketNo[0]);
	VECTOR v = VGet(0.0f, 0.0f, 0.0f);
	_cPos[0] = VTransform(v, m);

	MoveProcess();

	_iPos = _cPos[CHAIN_MAX - 1];
	MV1SetPosition(_iModelHandle, _iPos);
	_iForwardDir = VSub(_cPos[0], _iPos);
	if (VSize(_iForwardDir) > 0.0f) {
		if (_moveState != IB_MOVE_STATE::FOLLOWING) _iForwardDir = VScale(_iForwardDir, -1.0f);
		_iForwardDir = VNorm(_iForwardDir);
		Math::SetModelForward_RotationY(_iModelHandle, _iForwardDir);
	}
	_attackAnimCnt++;
	if (90 < _attackAnimCnt) {
		_attackAnimCnt = 0;
	}

	if (_iPos.y - _ibBodySphereCollision.r < 0.0f) {
		_iPos.y = 0.0f + _ibBodySphereCollision.r;
	}
	for (int i = 1; i < CHAIN_MAX; i++) {
		if (_cPos[i].y - 10.0f < 0.0f) {
			_cPos[i].y = 10.0f;
		}
	}

	UpdateIBCollision();
	_collisionManager->UpdateCell(_cell);
	if (_enabledAttackCollision) {
		UpdateChainCollision();
		_collisionManager->UpdateCell(_chainCell);
	}
	else {
		if (_chainCell->_segment != nullptr) {
			_collisionManager->ReserveRemovementCell(_chainCell);
		}
	}

	AnimProcess();
}

void IronBall::MoveProcess()
{
	switch (_moveState)
	{
	case FOLLOWING:
		FollowingProcess();
		break;
	case PUTTING_ON_SOCKET:
		PuttingOnSocketProcess();
		break;
	case INTERPOLATION:
		//InterpolationProcess();
		break;
	}
}

void IronBall::FollowingProcess()
{
	// 重力処理
	for (int i = 1; i < CHAIN_MAX; i++) {
		_cPos[i].y -= 12.0f;
		if (_cPos[i].y - 10.0f < 0.0f) {
			_cPos[i].y = 10.0f;
		}
	}
	_iPos.y -= 12.0f;
	if (_iPos.y < 0.0f) {
		_iPos.y = 0.0f;
	}

	//_length = 50.0f;
	for (int i = 0; i < CHAIN_MAX - 1; i++) {
		VECTOR vNext = _cPos[i + 1];
		VECTOR vDelta = VSub(vNext, _cPos[i]);
		float distance = VSize(vDelta);
		float difference = _length - distance;

		float offsetX = (difference * vDelta.x / distance) * 0.9f;
		float offsetY = (difference * vDelta.y / distance) * 0.9f;
		float offsetZ = (difference * vDelta.z / distance) * 0.9f;

		//if (i != 0) {
		//	_cPos[i].x -= offsetX;
		//	_cPos[i].y -= offsetY;
		//	_cPos[i].z -= offsetZ;
		//}
		float mul = 1.0f;
		if (i == 0) mul = 2.0f;
		_cPos[i + 1].x += offsetX * mul;
		_cPos[i + 1].y += offsetY * mul;
		_cPos[i + 1].z += offsetZ * mul;
	}
}

void IronBall::PuttingOnSocketProcess()
{
	// 各ソケットへの配置
	{
		VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
		MATRIX m = MGetIdent();

		// 鎖と腕輪の連結点
		m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, _socketNo[0]);
		_cPos[0] = VTransform(vOrigin, m);

		// 1つ目
		m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, _socketNo[1]);
		_cPos[1] = VTransform(vOrigin, m);

		// 鉄球の位置
		m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, _socketNo[2]);
		//VECTOR vTmp = VSub(VTransform(vOrigin, m), _pos[0]);
		//vTmp = VNorm(vTmp);
		//vTmp.y = 0.0f;
		//m = MMult(m, MGetTranslate(VScale(vTmp, 1000.0f)));

		_cPos[CHAIN_MAX - 1] = VTransform(vOrigin, m);
	}

	// キャラの座標から見た一つ目の鎖を配置する方向
	VECTOR vBase = VSub(_cPos[1], _cPos[0]);

	// キャラの座標から見た鉄球を配置する方向
	VECTOR vTarget = VSub(_cPos[CHAIN_MAX - 1], _cPos[0]);


	float rad = Math::CalcVectorAngle(vBase, vTarget);
	float dist = VSize(vTarget);
	VECTOR vCross = VCross(vBase, vTarget);
	const float chainNum = CHAIN_MAX - 1; ////////////////// （要修正）振り回している感じを出すなら、最後の鎖がピッタリ鉄球の接続位置に来るようにせず、少し引っ張っている方向にずらす必要がある
	//rad /= chainNum;
	for (int i = 1; i < CHAIN_MAX; i++) {
		VECTOR vTmp = VScale(VNorm(vBase), dist * ((float)(i - 1) / chainNum));
		MATRIX mRot = MGetRotAxis(vCross, rad * ((float)(i - 1) / chainNum));
		vTmp = VTransform(vTmp, mRot);
		_cPos[i] = VTransform(vTmp, MGetTranslate(_cPos[0]));

		if (_cPos[i].y < 0.0f) {
			_cPos[i].y = 0.0f;
		}
	}



	_cnt += 3.5f * (float)_attackDir;
	if (_cnt > 80.0f) {
		_attackDir = -1;
	}
	else if (_cnt < -80.0f) {
		_attackDir = 1;
	}
}

void IronBall::InterpolationProcess()
{
	// 各ソケットへの配置
	{
		VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
		MATRIX m = MGetIdent();

		// 鎖と腕輪の連結点
		m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, _socketNo[0]);
		_cPos[0] = VTransform(vOrigin, m);

		// 1つ目
		m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, _socketNo[1]);
		_cPos[1] = VTransform(vOrigin, m);

		// 鉄球の位置
		m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, _socketNo[2]);

		_cPos[CHAIN_MAX - 1] = VTransform(vOrigin, m);
	}

	// キャラの座標から見た一つ目の鎖を配置する方向
	VECTOR vBase = VSub(_cPos[1], _cPos[0]);

	// キャラの座標から見た鉄球を配置する方向
	VECTOR vTarget = VSub(_cPos[CHAIN_MAX - 1], _cPos[0]);


	float rad = Math::CalcVectorAngle(vBase, vTarget);
	float dist = VSize(vTarget);
	VECTOR vCross = VCross(vBase, vTarget);
	const float chainNum = CHAIN_MAX - 1; ////////////////// （要修正）振り回している感じを出すなら、最後の鎖がピッタリ鉄球の接続位置に来るようにせず、少し引っ張っている方向にずらす必要がある
	//rad /= chainNum;
	for (int i = 1; i < CHAIN_MAX; i++) {
		VECTOR vTmp = VScale(VNorm(vBase), dist * ((float)(i - 1) / chainNum));
		MATRIX mRot = MGetRotAxis(vCross, rad * ((float)(i - 1) / chainNum));
		vTmp = VTransform(vTmp, mRot);
		_cPos[i] = VTransform(vTmp, MGetTranslate(_cPos[0]));

		if (_cPos[i].y < 0.0f) {
			_cPos[i].y = 0.0f;
		}
	}
}

void IronBall::AnimProcess()
{
	MV1SetAttachAnimTime(_iModelHandle, _animIndex, _playTime);

	_playTime += 1.0f;	
	if (_animTotalTime < _playTime) {
		_playTime = 0.0f;
	}
}



void IronBall::Render()
{
	// 鎖の描画
	for (int i = 0; i < CHAIN_MAX; i++) {
		// モデルに座標を反映させる
		MV1SetPosition(_cModelHandle, _cPos[i]);
		// モデルの描画
		MV1DrawModel(_cModelHandle);
	}

	// 鉄球の描画
	MV1SetPosition(_iModelHandle, _iPos);
	MV1DrawModel(_iModelHandle);
}

void IronBall::UpdateIBCollision()
{
	_ibBodySphereCollision.centerPos = _iPos;
	_ibAttackSphereCollision.centerPos = _iPos;
}

void IronBall::UpdateChainCollision()
{
	_chainCapsuleCollision.up_pos = _cPos[0];
	_chainCapsuleCollision.down_pos = _cPos[CHAIN_MAX - 1];
}

void IronBall::SetPlayerModelHandle(int handle)
{
	_playerModelHandle = handle;

	_socketNo[0] = MV1SearchFrame(_playerModelHandle, "chain1");
	_socketNo[1] = MV1SearchFrame(_playerModelHandle, "chain2");
	_socketNo[2] = MV1SearchFrame(_playerModelHandle, "chain3");
}

void IronBall::DrawDebugInfo() {
	unsigned int color = _enabledAttackCollision ? COLOR_RED : COLOR_WHITE;
	_ibBodySphereCollision.Render(COLOR_GREEN);
	_ibAttackSphereCollision.Render(color);
	_chainCapsuleCollision.Render(color);

	//int x = 0;
	//int y = 0;
	//int line = 0;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_pos: x %3.2f, y %3.2f, z  %3.2f", _pos.x, _pos.y, _pos.z); line++;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_dir: x %3.2f, y %3.2f, z  %3.2f", _dir.x, _dir.y, _dir.z); line++;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_speed %3.2f", _speed); line++;
}

bool IronBall::UpdateLevel(float scale)
{
	MV1SetScale(_iModelHandle, VScale(_ibDefaultScale, scale));
	_ibBodySphereCollision.r = IB_BODY_COLLISION_RADIUS * scale;
	_ibAttackSphereCollision.r = IB_ATTACK_COLLISION_RADIUS * scale;
	return true;
}
