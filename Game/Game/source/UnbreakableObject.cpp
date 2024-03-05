#include "UnbreakableObject.h"

UnbreakableObject::UnbreakableObject()
{
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
}

UnbreakableObject::~UnbreakableObject()
{
}

void UnbreakableObject::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	// モデルの設定
	_modelHandle = modelHandle;
	_pos = startPos;
	MV1SetPosition(_modelHandle, _pos);
	MV1SetRotationXYZ(_modelHandle, rotation);
	MV1SetScale(_modelHandle, scale);

	// 当たり判定の設定
	// スケール
	_obbCol.length[0] = obbLength.x * scale.x;
	_obbCol.length[1] = obbLength.y * scale.y;
	_obbCol.length[2] = obbLength.z * scale.z;
	// 回転
	MATRIX mRot = MGetRotX(rotation.x);
	mRot = MMult(mRot, MGetRotY(rotation.y));
	mRot = MMult(mRot, MGetRotZ(rotation.z));
	_obbCol.dir_vec[0] = VTransform(_obbCol.dir_vec[0], mRot);
	_obbCol.dir_vec[1] = VTransform(_obbCol.dir_vec[1], mRot);
	_obbCol.dir_vec[2] = VTransform(_obbCol.dir_vec[2], mRot);
	// 座標
	_obbCol.pos = VAdd(startPos, VGet(0.0f, _obbCol.length[1] / 2.0f, 0.0f));
}

void UnbreakableObject::Process()
{
}

void UnbreakableObject::Render()
{
	MV1DrawModel(_modelHandle);
}

void UnbreakableObject::DrawDebugInfo()
{
	_obbCol.Render(COLOR_WHITE);
}
