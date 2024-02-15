#include "Tower.h"

Tower::Tower()
{
	_isFalling = false;
	_prevFallCnt = -1;
	_fallCnt = 0;
	_bottomIndex = 0;
	_endPos = VGet(0.0f, 0.0f, 0.0f);

	_r = 0;
	_up = 0.0f;
}

Tower::~Tower()
{
}

bool Tower::Init(std::array<int, 3> modelHandle, VECTOR startPos)
{
	_pos = startPos;
	_r = 120.0f;
	_up = 80.0f;
	for (int i = 0; i < 3; i++) {
		if (modelHandle[i] == -1) {
			return false;
		}

		TOWER_PARTS_INFO* pInfo = new TOWER_PARTS_INFO();
		pInfo->use = true;
		pInfo->blast = false;
		pInfo->modelHandle = modelHandle[i];
		if (i == 0) {
			pInfo->pos = startPos;
		}
		else {
			VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
			MATRIX m =  MV1GetFrameLocalWorldMatrix(_partsInfo[i - 1]->modelHandle, 3);
			pInfo->pos = VTransform(vOrigin, m);
		}
		MV1SetPosition(pInfo->modelHandle, pInfo->pos);

		pInfo->vRot = VGet(0.0f, 0.0f, 0.0f);
		_partsInfo.push_back(pInfo);

		Sphere* sphere = new Sphere();
		sphere->centerPos = VAdd(pInfo->pos, VGet(0.0f, _up, 0.0f));
		sphere->r = _r;
		_sphereCollision.push_back(sphere);
	}


	return true;
}

bool Tower::Process()
{
	BlastOffProcess();
	if(_isFalling) {

		int i = _bottomIndex;
		int size = _partsInfo.size();
		if (i < size) {
			_partsInfo[i]->pos.y -= 16.0f;

			if (_partsInfo[i]->pos.y < _endPos.y) {
				_partsInfo[i]->pos.y = _endPos.y;
			}

			i++;
			for (i; i < size; i++) {
				VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
				MATRIX m = MV1GetFrameLocalWorldMatrix(_partsInfo[i - 1]->modelHandle, 3);
				_partsInfo[i]->pos = VTransform(vOrigin, m);
			}

		}




		_fallCnt--;
		if (_fallCnt < 0) {
			_fallCnt = 0;
			_isFalling = false;
		}
	}





	if (_prevFallCnt >= 0) {
		_prevFallCnt--;
		if (_prevFallCnt < 0) {
			_isFalling = true;
			_fallCnt = 60;
		}
	}

	UpdateCollision();
	return true;
}

bool Tower::Render()
{
	bool successAll = true;
	for (auto itr = _partsInfo.begin(); itr != _partsInfo.end(); ++itr) {
		MV1SetPosition((*itr)->modelHandle, (*itr)->pos);
		int success = MV1DrawModel((*itr)->modelHandle);
		if (success == -1) {
			successAll = false;
		}
	}
	return successAll;
}

void Tower::BlastOffProcess()
{
	for(auto itr = _partsInfo.begin(); itr != _partsInfo.end(); ++itr) {
		if((*itr)->use && (*itr)->blast) {
			(*itr)->pos = VAdd((*itr)->pos, VScale((*itr)->blastDir, 60.0f));
		}
	}
}

void Tower::SetFalling(VECTOR vDir)
{
	_prevFallCnt = 45;
	for(auto itr = _partsInfo.begin(); itr != _partsInfo.end(); ++itr) {
		if ((*itr)->use && (*itr)->blast == false) {
			(*itr)->blast = true;
			_endPos = (*itr)->pos;
			(*itr)->blastDir = VNorm(vDir);
			_bottomIndex++;
			break;
		}
	}
}

void Tower::UpdateCollision()
{
	for(int i = 0;i < _partsInfo.size();i++) {
		_sphereCollision[i]->centerPos = VAdd(_partsInfo[i]->pos, VGet(0.0f, _up, 0.0f));
	}
}

bool Tower::DrawDebugInfo()
{
	if (!base::DrawDebugInfo()) { return false; }

	for(auto itr = _sphereCollision.begin(); itr != _sphereCollision.end(); ++itr) {
		DrawSphere3D((*itr)->centerPos, (*itr)->r, 16, COLOR_WHITE, COLOR_WHITE, false);
	}
	return true;
}
