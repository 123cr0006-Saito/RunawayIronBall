#include "Chain.h"


void Chain::Init() {
	_input = XInput::GetInstance();

	_cModelHandle = MV1LoadModel("res/Chain/Chain02.mv1");
	_cPos[0] = VGet(0.0f, 0.0f, 0.0f);
	MV1SetPosition(_cModelHandle, _cPos[0]);
	MV1SetScale(_cModelHandle, VGet(0.5f, 0.5f, 0.5f));

	for (int i = 1; i < CHAIN_MAX; i++) {
		_cPos[i] = VAdd(_cPos[i - 1], VGet(0.0f, 0.0f, -100.0f));
	}

	_iModelHandle = MV1LoadModel("res/Character/meatball/cg_player_meatball.mv1");
	_iPos = VAdd(_cPos[CHAIN_MAX - 1], VGet(0.0f, 10.0f, 0.0f));
	MV1SetPosition(_iModelHandle, _iPos);
	MV1SetScale(_iModelHandle, VGet(3.0f, 3.0f, 3.0f));

	MV1SetScale(_iModelHandle, VGet(1.0,1.0,1.0));

	_animIndex = MV1AttachAnim(_iModelHandle, 0);
	_animTotalTime = MV1GetAnimTotalTime(_iModelHandle, _animIndex);
	_playTime = 0.0f;

	//齋藤が書きました------------------------------------------------------------------
	SetPowerScale("res/JsonFile/IronState.json");
	//------------------------------------------------------------------
	_iForwardDir = VGet(0, 0, 0);


	_attackAnimCnt = 0;

	_cnt = 0.0f;
	_attackDir = 1;

	for (int i = 0; i < CHAIN_MAX; i++) {
		_m[i] = MGetIdent();
	}

	_length = 50.0f;

	_isSwing = false;


	_playerInstance = Player::GetInstance();
	_playerModelHandle = _playerInstance->GetModelHandle();
}

void Chain::SetPowerScale(std::string FileName) {
	myJson json(FileName);
	int level = 0;
	int power = 0;
	float scale = 0;
	for (auto& list : json._json) {
		list.at("Level").get_to(level);
		list.at("Power").get_to(power);
		list.at("Magnification").get_to(scale);
		_powerAndScale[level] = std::make_pair(power,scale);
	}
};

bool Chain::UpdateLevel() {
	static int _oldLevel;//前フレームのレベルです。
	int level = _playerInstance->GetNowLevel();

	if (_oldLevel != level) {
		_power = _powerAndScale[level].first;
		MV1SetScale(_iModelHandle, VScale(VGet(1,1,1),_powerAndScale[level].second));
		_r = _originR * _powerAndScale[level].second;
	}

	_oldLevel = level;
	return true;
};

void Chain::Process(VECTOR playerPos) {
	_isSwing = _playerInstance->GetIsSwing();

	_cPos[0] = playerPos;
	if (_isSwing) {
		/* デバッグ用 */
		{
			VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
			MATRIX m = MGetIdent();
	
			// 鎖と腕輪の連結点
			m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, 76);
			_cPos[0] = VTransform(vOrigin, m);
	
			// 1つ目
			m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, 77);
			_cPos[1] = VTransform(vOrigin, m);
	
			// 鉄球の位置
			m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, 78);
			//VECTOR vTmp = VSub(VTransform(vOrigin, m), _pos[0]);
			//vTmp = VNorm(vTmp);
			//vTmp.y = 0.0f;
			//m = MMult(m, MGetTranslate(VScale(vTmp, 1000.0f)));

			_cPos[CHAIN_MAX - 1] = VTransform(vOrigin, m);
		}
		/* */

		// キャラの座標から見た一つ目の鎖を配置する方向
		VECTOR vBase = VSub(_cPos[1], _cPos[0]);

		// キャラの座標から見た鉄球を配置する方向
		VECTOR vTarget = VSub(_cPos[CHAIN_MAX - 1], _cPos[0]);


		float rad = Math::CalcVectorAngle(vBase, vTarget);
		float dist = VSize(vTarget);
		VECTOR vCross = VCross(vBase, vTarget);
		const float chainNum = CHAIN_MAX - 1; ////////////////// （要修正）振り回している感じを出すなら、最後の鎖がピッタリ鉄球の接続位置に来るようにせず、少し引っ張っている方向にずらす必要がある
		//rad /= chainNum;
		for (int i = 1; i < CHAIN_MAX; i++) {
			VECTOR vTmp = VScale(VNorm(vBase), dist * ((float)(i - 1) / chainNum));
			MATRIX mRot = MGetRotAxis(vCross, rad * ((float)(i - 1) / chainNum));
			vTmp = VTransform(vTmp, mRot);
			_cPos[i] = VTransform(vTmp, MGetTranslate(_cPos[0]));

			if (_cPos[i].y < 0.0f) {
				_cPos[i].y = 0.0f;
			}
		}
	


		_cnt += 3.5f * (float)_attackDir;
		if (_cnt > 80.0f) {
			_attackDir = -1;
		}
		else if (_cnt < -80.0f) {
			_attackDir = 1;
		}
	}
	else {
		// 重力処理
		for (int i = 1; i < CHAIN_MAX; i++) {
			_cPos[i].y -= 12.0f;
			if (_cPos[i].y < 0.0f) {
				_cPos[i].y = 0.0f;
			}
		}
		_iPos.y -= 12.0f;
		if (_iPos.y < 0.0f) {
			_iPos.y = 0.0f;
		}

		//_length = 50.0f;
		for (int i = 0; i < CHAIN_MAX - 1; i++) {
			VECTOR vNext = _cPos[i + 1];
			VECTOR vDelta = VSub(vNext, _cPos[i]);
			float distance = VSize(vDelta);
			float difference = _length - distance;

			float offsetX = (difference * vDelta.x / distance) * 0.9f;
			float offsetY = (difference * vDelta.y / distance) * 0.9f;
			float offsetZ = (difference * vDelta.z / distance) * 0.9f;

			if (i != 0) {
				_cPos[i].x -= offsetX;
				_cPos[i].y -= offsetY;
				_cPos[i].z -= offsetZ;
			}
			float mul = 1.0f;
			if (i == 0) mul = 2.0f;
			_cPos[i + 1].x += offsetX * mul;
			_cPos[i + 1].y += offsetY * mul;
			_cPos[i + 1].z += offsetZ * mul;
		}

	}

	
	UpdateLevel();

	_iPos = _cPos[CHAIN_MAX - 1];
	MV1SetPosition(_iModelHandle, _iPos);
	_iForwardDir = VSub(_cPos[0], _iPos);
	if (VSize(_iForwardDir) > 0.0f) {
		if (_isSwing) _iForwardDir = VScale(_iForwardDir, -1.0f);
		_iForwardDir = VNorm(_iForwardDir);
		Math::SetModelForward_RotationY(_iModelHandle, _iForwardDir);
	}
	_attackAnimCnt++;
	if (90 < _attackAnimCnt) {
		_attackAnimCnt = 0;
	}

	AnimProcess();
}

void Chain::AnimProcess()
{
	MV1SetAttachAnimTime(_iModelHandle, _animIndex, _playTime);

	_playTime += _attackAnimCnt < 60 ? 10.0f : 1.0f;	
	if (_animTotalTime < _playTime) {
		_playTime = 0.0f;
	}
}



void Chain::Render()
{
	for (int i = 0; i < CHAIN_MAX; i++) {
		// モデルに座標を反映させる
		MV1SetPosition(_cModelHandle, _cPos[i]);
		MV1DrawModel(_cModelHandle);
	}

	MV1DrawModel(_iModelHandle);
}

void Chain::DrawDebugInfo() {
	DrawSphere3D(_iPos, _r, 16, COLOR_RED, COLOR_RED, false);


	//int x = 0;
	//int y = 0;
	//int line = 0;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_pos: x %3.2f, y %3.2f, z  %3.2f", _pos.x, _pos.y, _pos.z); line++;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_dir: x %3.2f, y %3.2f, z  %3.2f", _dir.x, _dir.y, _dir.z); line++;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_speed %3.2f", _speed); line++;
}
