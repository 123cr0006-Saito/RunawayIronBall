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
	_r = 100.0f;
	_up = 60.0f;
	for (int i = 0; i < 3; i++) {
		if (modelHandle[i] == -1) {
			return false;
		}

		//TOWER_PARTS_INFO* tpInfo = new TOWER_PARTS_INFO();
		//tpInfo->use = true;
		//tpInfo->blast = false;
		//tpInfo->modelHandle = modelHandle[i];
		//if (i == 0) {
		//	tpInfo->pos = startPos;
		//}
		//else {
		//	VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
		//	MATRIX m =  MV1GetFrameLocalWorldMatrix(_partsInfo[i - 1]->modelHandle, 3);
		//	tpInfo->pos = VTransform(vOrigin, m);
		//}
		//MV1SetPosition(tpInfo->modelHandle, tpInfo->pos);

		//tpInfo->vRot = VGet(0.0f, 0.0f, 0.0f);
		//_partsInfo.push_back(tpInfo);


		TowerParts* tp = new TowerParts();

		VECTOR tmpPos = VGet(0.0f, 0.0f, 0.0f);
		if (i == 0) {
			tmpPos = startPos;
		}
		else {
			VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
			MATRIX m = MV1GetFrameLocalWorldMatrix(_towerParts[i - 1]->_modelHandle, 3);
			tmpPos = VTransform(vOrigin, m);
		}
		
		tp->Init(modelHandle[i], tmpPos);
		_towerParts.push_back(tp);
	}


	//for (int i = 0; i < 3; i++) {
	//	TowerParts* pTowerParts = new TowerParts();
	//	pTowerParts->Init(_partsInfo[i]->modelHandle, _partsInfo[i]->pos);
	//	_towerParts.push_back(pTowerParts);
	//}

	return true;
}

bool Tower::Process()
{
	BlastOffProcess();
	if(_isFalling) {

		int i = _bottomIndex;
		int size = _towerParts.size();
		if (i < size) {
			_towerParts[i]->_pos.y -= 16.0f;

			if (_towerParts[i]->_pos.y < _endPos.y) {
				_towerParts[i]->_pos.y = _endPos.y;
			}

			i++;
			for (i; i < size; i++) {
				VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
				MATRIX m = MV1GetFrameLocalWorldMatrix(_towerParts[i -1]->_modelHandle, 3);
				_towerParts[i]->_pos = VTransform(vOrigin, m);
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
	for (auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
		(*itr)->Process();
	}
	return true;
}

bool Tower::Render()
{
	bool successAll = true;
	//for (auto itr = _partsInfo.begin(); itr != _partsInfo.end(); ++itr) {
	//	MV1SetPosition((*itr)->modelHandle, (*itr)->pos);
	//	int success = MV1DrawModel((*itr)->modelHandle);
	//	if (success == -1) {
	//		successAll = false;
	//	}
	//}

	for (auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
		(*itr)->Render();
	}
	return successAll;
}

void Tower::BlastOffProcess()
{
	for(auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
		if((*itr)->_use && (*itr)->_blast) {
			(*itr)->_pos = VAdd((*itr)->_pos, VScale((*itr)->_blastDir, 60.0f));
		}
	}
}

void Tower::SetFalling(VECTOR vDir)
{
	if (_prevFallCnt < 0 && !_isFalling) {
		_prevFallCnt = 45;
		for (auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
			if ((*itr)->_use && (*itr)->_blast == false) {
				(*itr)->_blast = true;
				_endPos = (*itr)->_pos;
				(*itr)->_blastDir = VNorm(vDir);
				_bottomIndex++;
				break;
			}
		}
	}
}

void Tower::UpdateCollision()
{
	//for(int i = 0;i < _partsInfo.size();i++) {
	//	_sphereCollision[i]->centerPos = VAdd(_partsInfo[i]->pos, VGet(0.0f, _up, 0.0f));
	//}
}

bool Tower::DrawDebugInfo()
{
	if (!base::DrawDebugInfo()) { return false; }

	for(auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
		DrawSphere3D((*itr)->_sphereCollision.centerPos, (*itr)->_sphereCollision.r, 16, COLOR_WHITE, COLOR_WHITE, false);
	}
	return true;
}
