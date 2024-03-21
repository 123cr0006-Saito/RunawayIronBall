#include "TowerParts.h"

namespace {
	constexpr float BLAST_SPEED = 60.0f;
	constexpr int BLAST_CNT_MAX = 180;
	constexpr float FALL_CNT_MAX = 30;
}

std::vector<TowerParts*> TowerParts::_blastTowerParts;
TowerParts::TowerParts()
{
	_use = true;
	_useCollision = false;

	_blast = false;
	_blastDir = VGet(0.0f, 0.0f, 0.0f);
	_blastCnt = 0;

	_isFalling = false;
	_fallCnt = 0;
	_fallStartPos = VGet(0.0f, 0.0f, 0.0f);
	_fallEndPos = VGet(0.0f, 0.0f, 0.0f);

	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
	_vRot = VGet(0.0f, 0.0f, 0.0f);

	_sphereCollision.centerPos = VGet(0.0f, 0.0f, 0.0f);
	_sphereCollision.r = 0.0f;

	_localCenterPos = VGet(0.0f, 0.0f, 0.0f);
}

TowerParts::~TowerParts()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
		_modelHandle = -1;
	}
}

void TowerParts::Init(int modelHandle, VECTOR startPos)
{
	_modelHandle = modelHandle;
	_pos = startPos;
	MV1SetPosition(_modelHandle, _pos);

	//VECTOR pivotLocalPos = VGet(0.0f, 0.0f, 0.0f);
	//VECTOR topLocalPos = VTransform(pivotLocalPos, MV1GetFrameLocalMatrix(_modelHandle, 3));

	//{
	//	VECTOR vDir = VSub(topLocalPos, pivotLocalPos);
	//	float length = VSize(vDir);
	//	vDir = VNorm(vDir);
	//	_localCenterPos = VAdd(pivotLocalPos, VScale(vDir, length / 2.0f));
	//}

	//{
	//	VECTOR pivotWorldPos = VTransform(pivotLocalPos, MV1GetLocalWorldMatrix(_modelHandle));
	//	VECTOR topWorldPos = VTransform(pivotLocalPos, MV1GetFrameLocalWorldMatrix(_modelHandle, 3));
	//	VECTOR vDir = VSub(topWorldPos, pivotWorldPos);
	//	float length = VSize(vDir);

	//	_sphereCollision.r = length / 2.0f;
	//}
	_sphereCollision.r = 250.0f;


	UpdateCollision();
}

void TowerParts::Process()
{
	if (_use) {
		// 吹っ飛び処理
		if ( _blast) {
			BlastOffProcess();
		}

		// 落下処理
		if (_isFalling) {
			FallProcess();
		}

		// モデルの座標を更新
		MV1SetPosition(_modelHandle, _pos);

		// 当たり判定の更新
		//if (_useCollision) {
			UpdateCollision();
		//}
	}
}

void TowerParts::BlastOffProcess()
{
	_pos = VAdd(_pos, VScale(_blastDir, BLAST_SPEED));
	_blastCnt++;
	if (_blastCnt > BLAST_CNT_MAX) {
		_use = false;
	}
}

void TowerParts::FallProcess()
{
	float x = Easing::InQuint(_fallCnt, _fallStartPos.x, _fallEndPos.x, FALL_CNT_MAX);
	float y = Easing::InQuint(_fallCnt, _fallStartPos.y, _fallEndPos.y, FALL_CNT_MAX);
	float z = Easing::InQuint(_fallCnt, _fallStartPos.z, _fallEndPos.z, FALL_CNT_MAX);
	_pos = VGet(x, y, z);

	_fallCnt++;
	if (_fallCnt > FALL_CNT_MAX) {
		_isFalling = false;
	}
}

void TowerParts::Render()
{
	if (_use) {
		MV1SetPosition(_modelHandle, _pos);
		MV1DrawModel(_modelHandle);
	}
}

void TowerParts::UpdateCollision()
{
	//MV1SetPosition(_modelHandle, _pos);

	//VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
	//VECTOR pivotWorldPos = VTransform(vOrigin, MV1GetLocalWorldMatrix(_modelHandle));
	//VECTOR topWorldPos = VTransform(vOrigin, MV1GetFrameLocalWorldMatrix(_modelHandle, 3));
	//VECTOR vDir = VSub(topWorldPos, pivotWorldPos);
	//vDir = VNorm(vDir);

	_sphereCollision.centerPos =_pos;
}
