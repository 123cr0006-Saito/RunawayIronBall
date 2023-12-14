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


	_attackCnt = 0;

	_cnt = 0.0f;

	for (int i = 0; i < CHAIN_MAX; i++) {
		_m[i] = MGetIdent();
	}

	_length = 80.0f;
}

void Chain::Process(VECTOR playerPos) {
//	float gravity = 8.0f;
//
//
//	VECTOR vOld = VSub(_pos[1], _pos[0]);
//	VECTOR vBase = VGet(0.0f, 0.0f, -100.0f);
//
//#if 0
//	vBase = VTransform(vBase, MGetRotY(DegToRad(40.0f) * sin(_cnt)));  // wave
//	_pos[1] = vBase;
//	_cnt += (2 * DX_PI) / (60.0f); // wave
//#else
//	vBase = VTransform(vBase, MGetRotY(DegToRad(_cnt))); // swing
//	//_pos[1] = vBase;
//	_cnt += 8.0f;					 // swing
//#endif	

	//VECTOR vDir = VSub(_pos[1], _pos[0]);
	//MATRIX mRot = MGetRotY(DegToRad(_cnt))/*MGetRotVec2(VGet(0.0f, 0.0f, -100.0f), vDir)*/;

	//for (int i = CHAIN_MAX - 1; 0 < i; i--) {
	//	_m[i] = _m[i - 1];
	//}
	//_m[0] = mRot;

	//for (int i = 1; i < CHAIN_MAX; i++) {
	//	VECTOR vTmp = VGet(0.0f, 0.0f, -100.0f * i);
	//	_pos[i] = VTransform(vTmp, MMult(_m[i], MGetTranslate(_pos[0])));
	//}

	//// リセット
	//if (CheckHitKey(KEY_INPUT_Z) != 0) {
	//	_pos[0] = VGet(0, 0, 0);
	//	for (int i = 0; i < CHAIN_MAX; i++) {
	//		_m[i] = MGetIdent();
	//		if (i > 0) {
	//			_pos[i] = VAdd(_pos[i - 1], VGet(0.0f, 0.0f, -100.0f));
	//		}
	//	}
	//	_cnt = 0.0f;
	//}

	_pos[0] = playerPos;

	//for (int i = 1; i < CHAIN_MAX; i++) {
	//	if (_cnt / 60.0f < i) break;
	//	VECTOR vDir = VSub(_pos[i], _pos[i - 1]);
	//	vDir.y = 0.0f;
	//	if (VSize(vDir) > 0.0f) {
	//		vDir = VNorm(vDir);
	//		vDir = VTransform(vDir, MGetRotY(90));
	//		_pos[i] = VAdd(_pos[i], VScale(vDir, 60.0f));
	//	}		
	//}
	//_cnt++;

	//VECTOR vTmp = VGet(0, 0, 0);
	//MATRIX mTranslate = MGetTranslate(VGet())


	//bool isMove = false;
	//VECTOR vDir = VGet(gamePad._lStick.x, -gamePad._lStick.y, 0);
	//if (VSize(vDir) > 0.0f) {
	//	vDir = VNorm(vDir);
	//	_pos[0] = VAdd(_pos[0], VScale(vDir, 10.0f));
	//	isMove = true;
	//}

	//_pos[0] = VGet(0, 500, 0);

	//VECTOR vTmp[CHAIN_MAX];
	//for (int i = 1; i < CHAIN_MAX; i++) {
	//	vTmp[i] = _pos[i];

	//	//if (i != 1 || isMove == true) {
	//		VECTOR vSub = VSub(_pos[i], _oldPos[i]);
	//		if (VSize(vSub) > 0.0f) {
	//			_pos[i] = VAdd(_pos[i], vSub);

	//		}
	//	//}

	//		//_pos[i].y -= 1.0f;
	//	_oldPos[i] = vTmp[i];
	//}

	// 重力処理
	for (int i = 1; i < CHAIN_MAX - 1; i++) {
		_pos[i].y -= 12.0f;
		if (_pos[i].y < 0.0f) {
			_pos[i].y = 0.0f;
		}
	}

	_length = 50.0f;
	for (int i = 0; i < CHAIN_MAX - 1; i++) {
		VECTOR vDelta = VSub(_pos[i + 1], _pos[i]);
		float distance = VSize(vDelta);
		float difference = _length - distance;

		float offsetX = (difference * vDelta.x / distance) * 0.5f;
		float offsetY = (difference * vDelta.y / distance) * 0.5f;
		float offsetZ = (difference * vDelta.z / distance) * 0.5f;

		if (i != 0) {
			_pos[i].x -= offsetX;
			_pos[i].y -= offsetY;
			_pos[i].z -= offsetZ;
		}
		_pos[i + 1].x += offsetX;
		_pos[i + 1].y += offsetY;
		_pos[i + 1].z += offsetZ;
	}















	//// 外側から1つ内側のモデルを引っ張る
	//for (int i = CHAIN_MAX - 1; 1 < i; i--) {
	//	VECTOR sub = VSub(_pos[i], _pos[i - 1]);
	//	while (VSize(sub) > 100.0f) {
	//		_pos[i - 1] = VAdd(_pos[i - 1], VNorm(sub));
	//		sub = VSub(_pos[i], _pos[i - 1]);
	//	}
	//}

	//// 内側から1つ外側のモデルを引っ張る
	//for (int i = 0; i < CHAIN_MAX - 1; i++) {
	//	VECTOR sub = VSub(_pos[i + 1], _pos[i]);
	//	while (VSize(sub) > 100.0f) {
	//		_pos[i + 1] = VAdd(_pos[i + 1], VNorm(sub));
	//		sub = VSub(_pos[i], _pos[i + 1]);
	//	}
	//}

	// モデルに座標を反映させる
	for (int i = 0; i < CHAIN_MAX; i++) {
		MV1SetPosition(_modelHandle[i], _pos[i]);
	}

	_mbDir = VSub(_pos[0], _pos[CHAIN_MAX - 1]);
	if (VSize(_mbDir) > 0.0f) {
		_mbDir = VNorm(_mbDir);
		Math::SetModelForward_RotationY(_modelHandle[CHAIN_MAX - 1], _mbDir);
	}
	_attackCnt++;
	if (90 < _attackCnt) {
		_attackCnt = 0;
	}

	AnimProcess();
}

void Chain::AnimProcess()
{
	MV1SetAttachAnimTime(_modelHandle[CHAIN_MAX - 1], _animIndex, _playTime);

	_playTime += _attackCnt < 60 ? 10.0f : 1.0f;	
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
