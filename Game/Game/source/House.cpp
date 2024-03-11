#include "House.h"

House::House()
{
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
	_useCollision = true;
	_breakObj = nullptr;
}

House::~House()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
	SAFE_DELETE(_breakObj);
}

void House::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	// ���f���̐ݒ�
	_modelHandle = modelHandle;
	_pos = startPos;
	MV1SetPosition(_modelHandle, _pos);
	MV1SetRotationXYZ(_modelHandle, rotation);
	MV1SetScale(_modelHandle, scale);

	// �����蔻��̐ݒ�
	// �X�P�[��
	obb.length[0] = obbLength.x * scale.x;
	obb.length[1] = obbLength.y * scale.y;
	obb.length[2] = obbLength.z * scale.z;
	// ��]
	MATRIX mRot = MGetRotX(rotation.x);
	mRot = MMult(mRot, MGetRotY(rotation.y));
	mRot = MMult(mRot, MGetRotZ(rotation.z));
	obb.dir_vec[0] = VTransform(obb.dir_vec[0], mRot);
	obb.dir_vec[1] = VTransform(obb.dir_vec[1], mRot);
	obb.dir_vec[2] = VTransform(obb.dir_vec[2], mRot);
	// ���W
	obb.pos = VAdd(startPos, VGet(0.0f, obb.length[1] / 2.0f, 0.0f));

	// �j�󏈗��N���X�̏�����
	_breakObj = NEW BreakObject();
	_breakObj->Init(_modelHandle);
}

void House::Process()
{
	_breakObj->Process();
}

void House::Render()
{
	MV1DrawModel(_modelHandle);
}

void House::SetHit(VECTOR vDir)
{
	// �j�󏈗��̊J�n
	ActivateBreakObject(true, vDir);
}

void House::ActivateBreakObject(bool activate, VECTOR vDir)
{
	_breakObj->Activate(activate, vDir);
	SetUseCollision(false);
}

void House::DrawDebugInfo()
{
	if (_useCollision) {
		obb.Render(GetColor(255, 255, 255));
	}
	_breakObj->DrawDebugInfo();
}
