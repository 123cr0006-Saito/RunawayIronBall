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
	/*	MV1DeleteModel(_modelHandle);
		_modelHandle = -1;*/
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
	_obbCollision.length[0] = obbLength.x * scale.x;
	_obbCollision.length[1] = obbLength.y * scale.y;
	_obbCollision.length[2] = obbLength.z * scale.z;
	// 回転
	MATRIX mRot = MGetRotX(rotation.x);
	mRot = MMult(mRot, MGetRotY(rotation.y));
	mRot = MMult(mRot, MGetRotZ(rotation.z));
	_obbCollision.dir_vec[0] = VTransform(_obbCollision.dir_vec[0], mRot);
	_obbCollision.dir_vec[1] = VTransform(_obbCollision.dir_vec[1], mRot);
	_obbCollision.dir_vec[2] = VTransform(_obbCollision.dir_vec[2], mRot);
	// 座標
	_obbCollision.pos = VAdd(startPos, VGet(0.0f, _obbCollision.length[1] / 2.0f, 0.0f));

	// セルの設定
	_cell->_objType = OBJ_TYPE::BLDG;
	_collisionManager->UpdateCell(_cell);
}

void BuildingBase::Process()
{

}

void BuildingBase::Render()
{
	MV1DrawModel(_modelHandle);
}

void BuildingBase::SetUseCollision(bool b)
{
	_useCollision = b;
	if (_useCollision) {
		_collisionManager->UpdateCell(_cell);
	}
	else {
		_collisionManager->ReserveRemovementCell(_cell);
	}
}

void BuildingBase::DrawDebugInfo()
{
	if (_useCollision) {
		_obbCollision.Render(GetColor(255, 255, 255));
	}
}
