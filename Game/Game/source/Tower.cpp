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
		delete* itr;
	}
	_towerParts.clear();

}

bool Tower::Init(std::array<int, 3> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale)
{
	_pos = startPos;
	for (int i = 0; i < 3; i++) {
		if (modelHandle[i] == -1) {
			return false;
		}

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

	_partsNum = _towerParts.size();
	return true;
}

bool Tower::Process()
{
	if (_use) {
		//if (_isFalling) {

		//	int i = _bottomIndex;
		//	if (i < _partsNum) {

		//		float cnt = FALL_CNT_MAX - _fallCnt;
		//		float x = Easing::InQuint(cnt, _startPos.x, _endPos.x, FALL_CNT_MAX);
		//		float y = Easing::InQuint(cnt, _startPos.y, _endPos.y, FALL_CNT_MAX);
		//		float z = Easing::InQuint(cnt, _startPos.z, _endPos.z, FALL_CNT_MAX);
		//		_towerParts[i]->_pos = VGet(x, y, z);
		//		MV1SetPosition(_towerParts[i]->_modelHandle, _towerParts[i]->_pos);

		//		i++;
		//		for (i; i < _partsNum; i++) {
		//			VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
		//			MATRIX m = MV1GetFrameLocalWorldMatrix(_towerParts[i - 1]->_modelHandle, 3);
		//			_towerParts[i]->_pos = VTransform(vOrigin, m);
		//			MV1SetPosition(_towerParts[i]->_modelHandle, _towerParts[i]->_pos);
		//		}

		//	}




		//	_fallCnt--;
		//	if (_fallCnt < 0) {
		//		_fallCnt = 0;
		//		_isFalling = false;
		//	}
		//}


		if (_isFalling) {
			bool fallingFinished = true;
			for (int i = _bottomIndex; i < _partsNum; i++) {
				if(_towerParts[i]->GetUse() == false) continue;

				fallingFinished = fallingFinished && !(_towerParts[i]->GetIsFalling());
			}

			if (fallingFinished) {
				_isFalling = false;
				_canBlast = true;
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

	for (int i = 0; i < _partsNum; i++) {
		_towerParts[i]->Render();
	}
	return successAll;
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

bool Tower::DrawDebugInfo()
{
	if (!base::DrawDebugInfo()) { return false; }

	for(auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
		DrawSphere3D((*itr)->_sphereCollision.centerPos, (*itr)->_sphereCollision.r, 16, COLOR_WHITE, COLOR_WHITE, false);
	}
	return true;
}
