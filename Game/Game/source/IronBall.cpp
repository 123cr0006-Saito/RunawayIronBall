//----------------------------------------------------------------------
// @filename IronBall.cpp
// @date: 2024/04/01
// @author: Morozumi Hiroya
// @explanation
// �v���C���[�����S���E���̐���E�`����s���N���X
//----------------------------------------------------------------------
#include "IronBall.h"

namespace {
	// �S����Sphere�R���W�����̔��a�i�n�ʂƂ̔���p�j
	constexpr float IB_BODY_COLLISION_RADIUS = 50.0f;
	// �S���̍U��Sphere�R���W�����̔��a
	constexpr float IB_ATTACK_COLLISION_RADIUS = 200.0f;
	// ���̍U��Capsule�R���W�����̔��a
	constexpr float CHAIN_COLLISION_RADIUS = 150.0f;
}

IronBall::IronBall()
{
	_cModelHandle = -1;
	_iModelHandle = -1;
	_parentModelHandle = -1;
	_input = nullptr;
	_iForwardDir = VGet(0, 0, 0);

	for(int i = 0; i < CHAIN_MAX; i++) {
		_cPos[i] = VGet(0, 0, 0);
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
	_lengthBetweenChains = 0;
	_moveState = IB_MOVE_STATE::FOLLOWING;
	_enabledAttackCollision = false;
	_socketNo[0] = -1;
	_socketNo[1] = -1;
	_socketNo[2] = -1;
	_parent = nullptr;
}

IronBall::~IronBall()
{
	_input = nullptr;
	_parent = nullptr;
	delete _cell; _cell = nullptr;
	delete _chainCell; _chainCell = nullptr;
	for (auto list : _afterglowList) {
		delete list;
	}
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

	_lengthBetweenChains = 50.0f;

	_moveState = IB_MOVE_STATE::FOLLOWING;

	_enabledAttackCollision = false;

	_cell->_objType = OBJ_TYPE::PL_IB;
	
	_chainCell = NEW Cell();
	_chainCell->_obj = this;
	_chainCell->_objType = OBJ_TYPE::PL_IB_CHAIN;


	int afterglow = MV1SearchFrame(_iModelHandle, "left_eye02");
	_afterglowList.push_back(NEW Afterglow(_iModelHandle, afterglow, 10, LoadGraph("res/Effect/afterglow.png"), 20));
	afterglow = MV1SearchFrame(_iModelHandle, "right_eye02");
	_afterglowList.push_back(NEW Afterglow(_iModelHandle, afterglow, 10, LoadGraph("res/Effect/afterglow.png"), 20));
}

// �e���f���̍��W�����Ƃɏ����ʒu��ݒ�
void IronBall::InitPosFromParent()
{
	MATRIX m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[0]);
	VECTOR v = VGet(0.0f, 0.0f, 0.0f);
	_cPos[0] = VTransform(v, m);

	for (int i = 1; i < CHAIN_MAX; i++) {
		_cPos[i] = VAdd(_cPos[i - 1], VGet(0.0f, 0.0f, _lengthBetweenChains));
	}
	_iPos = _cPos[CHAIN_MAX - 1];
}


void IronBall::Process() {
	// �e���f���̎茳�ɔz�u���鍽�̍��W��ݒ�
	MATRIX m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[0]);
	VECTOR v = VGet(0.0f, 0.0f, 0.0f);
	_cPos[0] = VTransform(v, m);

	// �ړ�����
	MoveProcess();

	// �����n�ʂɖ��܂�Ȃ��悤�ɂ���
	for (int i = 1; i < CHAIN_MAX; i++) {
		if (_cPos[i].y - 10.0f < 0.0f) {
			_cPos[i].y = 10.0f;
		}
	}

	// �S���̍��W���X�V
	_iPos = _cPos[CHAIN_MAX - 1];
	// �S�����n�ʂɖ��܂�Ȃ��悤�ɂ���
	if (_iPos.y - _ibBodySphereCollision.r < 0.0f) {
		_iPos.y = 0.0f + _ibBodySphereCollision.r;
	}

	// �S���̐��ʕ�����ݒ�
	// �S�����猩�����Ɛe���f���̘A���_�̕����������悤�ɂ���
	_iForwardDir = VSub(_cPos[0], _iPos);
	if (VSquareSize(_iForwardDir) > 0.0f) {
		// �Ǐ]��ԏo�Ȃ����́A���ʕ������t�ɂ���
		if (_moveState != IB_MOVE_STATE::FOLLOWING) _iForwardDir = VScale(_iForwardDir, -1.0f);
		Math::SetModelForward_RotationY(_iModelHandle, _iForwardDir);
	}



	// �����蔻��̍X�V
	UpdateIBCollision();
	_collisionManager->UpdateCell(_cell);

	// �U�����肪�L���Ȃ獽�̓����蔻����X�V
	if (_enabledAttackCollision) {
		UpdateChainCollision();
		_collisionManager->UpdateCell(_chainCell);
	}
	// �����Ȃ獽�̓����蔻����폜
	else {
		if (_chainCell->_segment != nullptr) {
			_collisionManager->ReserveRemovementCell(_chainCell);
		}
	}

	// �A�j���[�V�����̍X�V
	AnimProcess();

	// �c���̏���
	for (auto list : _afterglowList) {
		list->Process();
	}
}

// �ړ�����
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

// �Ǐ]��Ԃ̏���
void IronBall::FollowingProcess()
{
	// �d�͏���
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

	// �����m�̊Ԋu�����ɕۂ�
	for (int i = 0; i < CHAIN_MAX - 1; i++) {
		VECTOR vNext = _cPos[i + 1];
		VECTOR vDelta = VSub(vNext, _cPos[i]);
		float distance = VSize(vDelta);
		float difference = _lengthBetweenChains - distance;

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

// �\�P�b�g�ւ̔z�u��Ԃ̏���
void IronBall::PuttingOnSocketProcess()
{
	// �e�\�P�b�g�ւ̔z�u
	// �z�u�ʒu�̓A�j���[�V�����Őݒ肳��Ă���
	{
		VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
		MATRIX m = MGetIdent();

		// 0�Ԗځi�e���f���̎茳�̈ʒu�j
		m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[0]);
		_cPos[0] = VTransform(vOrigin, m);

		// 1�Ԗځi0�ԖڂɂȂ��鍽�̈ʒu�j
		m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[1]);
		_cPos[1] = VTransform(vOrigin, m);

		// 2�Ԗځi�S���̈ʒu�j
		m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[2]);
		_cPos[CHAIN_MAX - 1] = VTransform(vOrigin, m);
	}

	// �c��̍��̈ʒu�̓v���O�����ŕ�Ԃ���

	// 0�Ԗڂ̍����猩��1�Ԗڂ̍���z�u�������
	VECTOR vBase = VSub(_cPos[1], _cPos[0]);
	// 0�Ԗڂ̍����猩��2�Ԗځi�S���j��z�u�������
	VECTOR vTarget = VSub(_cPos[CHAIN_MAX - 1], _cPos[0]);

	// 2�̃x�N�g���̊p�x�����߂�
	float rad = Math::CalcVectorAngle(vBase, vTarget);
	// 0�Ԗڂ�2�Ԗځi�S���j�̋������v�Z����
	float dist = VSize(vTarget);
	vBase = VNorm(vBase);
	VECTOR vCross = VCross(vBase, vTarget);
	const float chainNum = CHAIN_MAX - 1;
	for (int i = 1; i < CHAIN_MAX; i++) {
		VECTOR vTmp = VScale(vBase, dist * ((float)(i) / chainNum));
		MATRIX mRot = MGetRotAxis(vCross, rad * ((float)(i) / chainNum));
		vTmp = VTransform(vTmp, mRot);
		_cPos[i] = VTransform(vTmp, MGetTranslate(_cPos[0]));

		if (_cPos[i].y < 0.0f) {
			_cPos[i].y = 0.0f;
		}
	}
}

// ��ԏ�Ԃ̏���
void IronBall::InterpolationProcess()
{
	// �e�\�P�b�g�ւ̔z�u
	{
		VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
		MATRIX m = MGetIdent();

		// ���Ƙr�ւ̘A���_
		m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[0]);
		_cPos[0] = VTransform(vOrigin, m);

		// 1��
		m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[1]);
		_cPos[1] = VTransform(vOrigin, m);

		// �S���̈ʒu
		m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[2]);

		_cPos[CHAIN_MAX - 1] = VTransform(vOrigin, m);
	}

	// �L�����̍��W���猩����ڂ̍���z�u�������
	VECTOR vBase = VSub(_cPos[1], _cPos[0]);

	// �L�����̍��W���猩���S����z�u�������
	VECTOR vTarget = VSub(_cPos[CHAIN_MAX - 1], _cPos[0]);


	float rad = Math::CalcVectorAngle(vBase, vTarget);
	float dist = VSize(vTarget);
	VECTOR vCross = VCross(vBase, vTarget);
	const float chainNum = CHAIN_MAX - 1;
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

// �A�j���[�V�����̍X�V����
// �S���̃A�j���[�V������1��ނ݂̂Ȃ̂�AnimationManager�N���X���g�p���Ȃ�
void IronBall::AnimProcess()
{
	MV1SetAttachAnimTime(_iModelHandle, _animIndex, _playTime);

	_playTime += 1.0f;	
	if (_animTotalTime < _playTime) {
		_playTime = 0.0f;
	}
}

// �`�揈��
void IronBall::Render()
{
	// �c���̕`��
	for (auto list : _afterglowList) {
		list->Render();
	}
	// ���̕`��
	for (int i = 0; i < CHAIN_MAX - 1; i++) { //  �Ō�̗v�f�͓S���̍��W�ƂȂ邽��, i < CHAIN_MAX - 1
		// ���f���ɍ��W�𔽉f������
		MV1SetPosition(_cModelHandle, _cPos[i]);
		// ���f���̕`��
		MV1DrawModel(_cModelHandle);
	}

	// �S���̕`��
	MV1SetPosition(_iModelHandle, _iPos);
	MV1DrawModel(_iModelHandle);
}

// �S���̓����蔻����X�V
void IronBall::UpdateIBCollision()
{
	_ibBodySphereCollision.centerPos = _iPos;
	_ibAttackSphereCollision.centerPos = _iPos;
}

// ���̓����蔻����X�V
void IronBall::UpdateChainCollision()
{
	_chainCapsuleCollision.up_pos = _cPos[0];
	_chainCapsuleCollision.down_pos = _cPos[CHAIN_MAX - 1];
}

// �ڂ̎c����\�����邩�ǂ�����ݒ�
// �T�C�g�E���쐬���܂����B
void IronBall::SetEnabledAfterGlow(bool enable) {
	for (auto list : _afterglowList) {
		list->SetUpdate(enable);
	}
}

// �e�I�u�W�F�N�g�̃��f���n���h�����Z�b�g
void IronBall::SetParentModelHandle(int handle)
{
	_parentModelHandle = handle;

	// �S���E���̃��f����z�u����e���f���̃\�P�b�g�ԍ����擾
	_socketNo[0] = MV1SearchFrame(_parentModelHandle, "chain1");
	_socketNo[1] = MV1SearchFrame(_parentModelHandle, "chain2");
	_socketNo[2] = MV1SearchFrame(_parentModelHandle, "chain3");
}

// �f�o�b�O���̕\��
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
