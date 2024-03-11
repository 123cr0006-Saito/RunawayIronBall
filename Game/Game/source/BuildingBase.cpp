#include "BuildingBase.h"

BuildingBase::BuildingBase()
{
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
	_useCollision = true;
	_canBreak = false;
}

BuildingBase::~BuildingBase()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
}

void BuildingBase::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	// モデルの設定
	_modelHandle = modelHandle;
	_pos = startPos;
	MV1SetPosition(_modelHandle, _pos);
	MV1SetRotationXYZ(_modelHandle, rotation);
	MV1SetScale(_modelHandle, scale);

	// 当たり判定の設定
	// スケール
	obb.length[0] = obbLength.x * scale.x;
	obb.length[1] = obbLength.y * scale.y;
	obb.length[2] = obbLength.z * scale.z;
	// 回転
	MATRIX mRot = MGetRotX(rotation.x);
	mRot = MMult(mRot, MGetRotY(rotation.y));
	mRot = MMult(mRot, MGetRotZ(rotation.z));
	obb.dir_vec[0] = VTransform(obb.dir_vec[0], mRot);
	obb.dir_vec[1] = VTransform(obb.dir_vec[1], mRot);
	obb.dir_vec[2] = VTransform(obb.dir_vec[2], mRot);
	// 座標
	obb.pos = VAdd(startPos, VGet(0.0f, obb.length[1] / 2.0f, 0.0f));

	_cell->_objType = OBJ_TYPE::BLDG;
}

void BuildingBase::Process()
{

}

void BuildingBase::Render()
{
	MV1DrawModel(_modelHandle);
}

void BuildingBase::DrawDebugInfo()
{

}
