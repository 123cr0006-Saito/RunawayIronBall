#include "Chain.h"


void Chain::Init() {
	_input = XInput::GetInstance();

	_modelHandle[0] = MV1LoadModel("res/Chain/Chain02.mv1");
	_pos[0] = VGet(0.0f, 0.0f, 0.0f);
	MV1SetPosition(_modelHandle[0], _pos[0]);
	MV1SetScale(_modelHandle[0], VGet(0.5f, 0.5f, 0.5f));

	for (int i = 1; i < CHAIN_MAX - 1; i++) {
		_modelHandle[i] = MV1DuplicateModel(_modelHandle[0]);
		_pos[i] = VAdd(_pos[i - 1], VGet(0.0f, 0.0f, -100.0f));
		MV1SetPosition(_modelHandle[i], _pos[i]);
		MV1SetScale(_modelHandle[i], VGet(0.5f, 0.5f, 0.5f));
	}

	_modelHandle[CHAIN_MAX - 1] = MV1LoadModel("res/Character/meatball/cg_player_meatball.mv1");
	_pos[CHAIN_MAX - 1] = VAdd(_pos[CHAIN_MAX - 1 - 1], VGet(0.0f, 10.0f, 0.0f));
	MV1SetPosition(_modelHandle[CHAIN_MAX - 1], _pos[CHAIN_MAX - 1]);
	MV1SetScale(_modelHandle[CHAIN_MAX - 1], VGet(3.0f, 3.0f, 3.0f));

	_animIndex = MV1AttachAnim(_modelHandle[CHAIN_MAX - 1], 0);
	_animTotalTime = MV1GetAnimTotalTime(_modelHandle[CHAIN_MAX - 1], _animIndex);
	_playTime = 0.0f;



	_mbDir = VGet(0, 0, 0);


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

void Chain::Process(VECTOR playerPos) {
	_isSwing = _playerInstance->GetIsSwing();

	_pos[0] = playerPos;
	if (_isSwing) {
		/* デバッグ用 */
		{
			VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
			MATRIX m = MGetIdent();
	
			// 鎖と腕輪の連結点
			m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, 217);
			_pos[0] = VTransform(vOrigin, m);
	
			// 1つ目
			m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, 218);
			_pos[1] = VTransform(vOrigin, m);
	
			// 鉄球の位置
			m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, 219);
			//VECTOR vTmp = VSub(VTransform(vOrigin, m), _pos[0]);
			//vTmp = VNorm(vTmp);
			//vTmp.y = 0.0f;
			//m = MMult(m, MGetTranslate(VScale(vTmp, 1000.0f)));

			_pos[CHAIN_MAX - 1] = VTransform(vOrigin, m);
		}
		/* */

		// キャラの座標から見た一つ目の鎖を配置する方向
		VECTOR vBase = VSub(_pos[1], _pos[0]);

		// キャラの座標から見た鉄球を配置する方向
		VECTOR vTarget = VSub(_pos[CHAIN_MAX - 1], _pos[0]);


		float rad = Math::CalcVectorAngle(vBase, vTarget);
		float dist = VSize(vTarget);
		VECTOR vCross = VCross(vBase, vTarget);
		const float chainNum = CHAIN_MAX - 2; ////////////////// （要修正）振り回している感じを出すなら、最後の鎖がピッタリ鉄球の接続位置に来るようにせず、少し引っ張っている方向にずらす必要がある
		//rad /= chainNum;
		for (int i = 1; i < CHAIN_MAX - 1; i++) {
			VECTOR vTmp = VScale(VNorm(vBase), dist * ((float)(i - 1) / chainNum));
			MATRIX mRot = MGetRotAxis(vCross, rad * ((float)(i - 1) / chainNum));
			vTmp = VTransform(vTmp, mRot);
			_pos[i] = VTransform(vTmp, MGetTranslate(_pos[0]));

			if (_pos[i].y < 0.0f) {
				_pos[i].y = 0.0f;
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
	for (int i = 1; i < CHAIN_MAX - 1; i++) {
		_pos[i].y -= 12.0f;
		if (_pos[i].y < 0.0f) {
			_pos[i].y = 0.0f;
		}
	}

		//_length = 50.0f;
	for (int i = 0; i < CHAIN_MAX - 1; i++) {
		VECTOR vDelta = VSub(_pos[i + 1], _pos[i]);
		float distance = VSize(vDelta);
		float difference = _length - distance;

			float offsetX = (difference * vDelta.x / distance) * 0.8f;
			float offsetY = (difference * vDelta.y / distance) * 0.8f;
			float offsetZ = (difference * vDelta.z / distance) * 0.8f;

		if (i != 0) {
			_pos[i].x -= offsetX;
			_pos[i].y -= offsetY;
			_pos[i].z -= offsetZ;
		}
		_pos[i + 1].x += offsetX;
		_pos[i + 1].y += offsetY;
		_pos[i + 1].z += offsetZ;
	}

	}

	// モデルに座標を反映させる
	for (int i = 0; i < CHAIN_MAX; i++) {
		MV1SetPosition(_modelHandle[i], _pos[i]);
	}
	

	_mbDir = VSub(_pos[0], _pos[CHAIN_MAX - 1]);
	if (VSize(_mbDir) > 0.0f) {
		if (_isSwing) _mbDir = VScale(_mbDir, -1.0f);
		_mbDir = VNorm(_mbDir);
		Math::SetModelForward_RotationY(_modelHandle[CHAIN_MAX - 1], _mbDir);
	}
	_attackAnimCnt++;
	if (90 < _attackAnimCnt) {
		_attackAnimCnt = 0;
	}

	AnimProcess();
}

void Chain::AnimProcess()
{
	MV1SetAttachAnimTime(_modelHandle[CHAIN_MAX - 1], _animIndex, _playTime);

	_playTime += _attackAnimCnt < 60 ? 10.0f : 1.0f;	
	if (_animTotalTime < _playTime) {
		_playTime = 0.0f;
	}
}



void Chain::Render()
{
	for (int i = 0; i < CHAIN_MAX; i++) {
		MV1DrawModel(_modelHandle[i]);
	}
}

void Chain::DrawDebugInfo() {
	//for (int i = 0; i < CHAIN_MAX; i++) {
	//	VECTOR worldPos = VGet(0, 0, 0);
	//	int index = i != CHAIN_MAX - 1 ? 0 : 2;
	//	MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle[i], index);
	//	worldPos = VTransform(worldPos, m);
	//	//VECTOR screenPos = ConvWorldPosToScreenPos(MV1GetPosition(_modelHandle[i]));
	//	VECTOR screenPos = ConvWorldPosToScreenPos(worldPos);
	//	DrawFormatString(screenPos.x, screenPos.y, GetColor(255, 0, 0), "%d", i);
	//}

	int x = 0;
	int y = 0;
	int line = 0;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_pos: x %3.2f, y %3.2f, z  %3.2f", _pos.x, _pos.y, _pos.z); line++;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_dir: x %3.2f, y %3.2f, z  %3.2f", _dir.x, _dir.y, _dir.z); line++;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_speed %3.2f", _speed); line++;
}
