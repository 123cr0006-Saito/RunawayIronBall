#include "Tower.h"


namespace {
	constexpr int FALL_CNT_MAX = 30; 
}

Tower::Tower()
{
	_use = true;
	_partsNum = 0;
	_isFalling = false;
	_prevFallCnt = -1;
	_fallCnt = 0;

	_canBlast = true;

	_bottomIndex = 0;

	_bottomSphereCollision = nullptr;
}

Tower::~Tower()
{
	for (auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
		delete *itr;
	}
	_towerParts.clear();
}

void Tower::Init(std::array<int, 3> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale)
{
	_pos = startPos;
	for (int i = 0; i < 3; i++) {
		TowerParts* tp = NEW TowerParts();
		VECTOR tmpPos = VGet(0.0f, 0.0f, 0.0f);
		if (i == 0) {
			tmpPos = startPos;
		}
		else {
			VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
			MATRIX m = MV1GetFrameLocalWorldMatrix(_towerParts[i - 1]->_modelHandle, 1);
			tmpPos = VTransform(vOrigin, m);
		}
		
		tp->Init(modelHandle[i], tmpPos);
		_towerParts.push_back(tp);
	}

	_partsNum = _towerParts.size();

	_towerParts[0]->SetUseCollision(true);
}

void Tower::Process()
{
	if (_use) {
		// 落下処理
		if (_isFalling) {
			// 残っている全てのパーツが落下終了したかどうかを判定する
			bool finishedFalling = true;
			for (int i = _bottomIndex; i < _partsNum; i++) {
				if(_towerParts[i]->GetUse() == false) continue;

				finishedFalling = finishedFalling && !(_towerParts[i]->GetIsFalling());
			}
			// 全てのパーツが落下終了したら、落下処理を終了する
			if (finishedFalling) {
				_isFalling = false;
				_canBlast = true;
				_towerParts[_bottomIndex]->SetUseCollision(true);
			}
		}


		if (_prevFallCnt >= 0) {
			_prevFallCnt--;
			if (_prevFallCnt < 0) {
				_isFalling = true;
				_fallCnt = FALL_CNT_MAX;

				//for (auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
				//	if ((*itr)->_use && (*itr)->_blast == false) {
				//		_startPos = (*itr)->_pos;
				//		break;
				//	}
				//}
			}



		}

		UpdateCollision();

	}
	for (int i = 0; i < _partsNum; i++) {
		_towerParts[i]->Process();
	}
}

void Tower::Render()
{
	for (int i = 0; i < _partsNum; i++) {
		_towerParts[i]->Render();
	}
}

void Tower::SetBlast(VECTOR vDir)
{
	if (_prevFallCnt < 0 && !_isFalling) {
		_prevFallCnt = 45;
		_canBlast = false;

		for (int i = _bottomIndex; i < _partsNum; i++) {

			// 最下部のパーツのみ吹っ飛び処理
			if (i == _bottomIndex) {
				_towerParts[i]->SetBlast(VNorm(vDir));
				TowerParts::AddBlastTowerParts(_towerParts[i]);
			}
			// それ以外のパーツは落下処理
			else {
				// 一つしたのパーツの現在の座標を、落下終了後の座標として設定
				_towerParts[i]->SetFalling(_towerParts[i - 1]->_pos);
			}
		}
		_bottomIndex++;

		if (_bottomIndex >= _partsNum) {
			_use = false;
		}
	}
}

void Tower::UpdateCollision()
{
	//for(int i = 0;i < _partsInfo.size();i++) {
	//	_sphereCollision[i]->centerPos = VAdd(_partsInfo[i]->pos, VGet(0.0f, _up, 0.0f));
	//}
}

void Tower::DrawDebugInfo()
{
	for(auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
		DrawSphere3D((*itr)->_sphereCollision.centerPos, (*itr)->_sphereCollision.r, 16, COLOR_WHITE, COLOR_WHITE, false);
	}
}
