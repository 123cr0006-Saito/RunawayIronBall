#include "TowerParts.h"

TowerParts::TowerParts()
{
	_use = true;
	_useCollision = false;

	_blast = false;
	_blastDir = VGet(0.0f, 0.0f, 0.0f);

	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
	_vRot = VGet(0.0f, 0.0f, 0.0f);

	_sphereCollision.centerPos = VGet(0.0f, 0.0f, 0.0f);
	_sphereCollision.r = 0.0f;

	_localCenterPos = VGet(0.0f, 0.0f, 0.0f);
}

TowerParts::~TowerParts()
{
}

void TowerParts::Init(int modelHandle, VECTOR startPos)
{
	_modelHandle = modelHandle;
	_pos = startPos;
	MV1SetPosition(_modelHandle, _pos);

	VECTOR pivotLocalPos = VGet(0.0f, 0.0f, 0.0f);
	VECTOR topLocalPos = VTransform(pivotLocalPos, MV1GetFrameLocalMatrix(_modelHandle, 3));

	{
		VECTOR vDir = VSub(topLocalPos, pivotLocalPos);
		float length = VSize(vDir);
		vDir = VNorm(vDir);
		_localCenterPos = VAdd(pivotLocalPos, VScale(vDir, length / 2.0f));
	}

	{
		VECTOR pivotWorldPos = VTransform(pivotLocalPos, MV1GetLocalWorldMatrix(_modelHandle));
		VECTOR topWorldPos = VTransform(pivotLocalPos, MV1GetFrameLocalWorldMatrix(_modelHandle, 3));
		VECTOR vDir = VSub(topWorldPos, pivotWorldPos);
		float length = VSize(vDir);

		_sphereCollision.r = length / 2.0f;
	}
}

void TowerParts::Process()
{
	UpdateCollision();
}

void TowerParts::Render()
{
	MV1SetPosition(_modelHandle, _pos);
	//MV1DrawModel(_modelHandle);
}

void TowerParts::UpdateCollision()
{
	MV1SetPosition(_modelHandle, _pos);

	VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
	VECTOR pivotWorldPos = VTransform(vOrigin, MV1GetLocalWorldMatrix(_modelHandle));
	VECTOR topWorldPos = VTransform(vOrigin, MV1GetFrameLocalWorldMatrix(_modelHandle, 3));
	VECTOR vDir = VSub(topWorldPos, pivotWorldPos);
	vDir = VNorm(vDir);

	_sphereCollision.centerPos = VAdd(pivotWorldPos, VScale(vDir, _sphereCollision.r));
}
