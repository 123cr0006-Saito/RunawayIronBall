#include "LoadingChain.h"
LoadingChain::LoadingChain() {
	_cModelHandle = -1;
	_iModelHandle = -1;
	_playerModelHandle = -1;

	for (int i = 0; i < Chain_Num; i++) {
		_cPos[i] = VGet(0, 0, 0);
	}
	for (int i = 0; i < 3; i++) {
		_socketNo[i] = -1;
	}

	_iPos = VGet(0, 0, 0);
	_animIndex = 0;
	_animTotalTime = 0;
	_playTime = 0;
	_attackAnimCnt = 0;
	_length = 0;

};

LoadingChain::~LoadingChain() {
	delete _modelColor; _modelColor = nullptr;
	MV1DeleteModel(_cModelHandle);
	MV1DeleteModel(_iModelHandle);
};

void LoadingChain::Init(){
	_cModelHandle = ResourceServer::MV1LoadModel("LoadingChain","res/Character/Loading/Cg_Chain.mv1");
	_cPos[0] = VGet(0.0f, 0.0f, 0.0f);
	MV1SetPosition(_cModelHandle, _cPos[0]);
	MV1SetScale(_cModelHandle, VGet(0.5f, 0.5f, 0.5f));

	for (int i = 1; i < Chain_Num; i++) {
		_cPos[i] = VAdd(_cPos[i - 1], VGet(100.0f, 0.0f, 0.0f));
	}

	_iModelHandle = ResourceServer::MV1LoadModel("LoadingIronBall", "res/Character/Loading/Cg_Iron_Ball.mv1");
	_iPos = VAdd(_cPos[Chain_Num - 1], VGet(0.0f, 10.0f, 0.0f));
	_ibDefaultScale = VGet(2.5f, 2.5f, 2.5f);
	MV1SetScale(_iModelHandle, _ibDefaultScale);
	MV1SetPosition(_iModelHandle, _iPos);

	_animIndex = MV1AttachAnim(_iModelHandle, 0);
	_animTotalTime = MV1GetAnimTotalTime(_iModelHandle, _animIndex);
	_playTime = 0.0f;
	_attackAnimCnt = 0;

	_length = 50.0f;

	// 境界線を消す
	for (int i = 0; i < MV1GetMaterialNum(_cModelHandle); i++) {
		MV1SetMaterialOutLineWidth(_cModelHandle, i, 0);
		MV1SetMaterialOutLineDotWidth(_cModelHandle, i, 0);
	}
	for (int i = 0; i < MV1GetMaterialNum(_iModelHandle); i++) {
		MV1SetMaterialOutLineWidth(_iModelHandle, i, 0);
		MV1SetMaterialOutLineDotWidth(_iModelHandle, i, 0);
	}

	_modelColor = NEW ModelColor();
	_modelColor->Init(_iModelHandle);
	_modelColor->ChangeFlickerTexture(true);
	_modelColor->Init(_cModelHandle);
	_modelColor->ChangeFlickerTexture(true);
};

void LoadingChain::Process(){
	// 鎖の初めの位置
	MATRIX m = MV1GetFrameLocalWorldMatrix(_playerModelHandle, _socketNo[0]);
	VECTOR v = VGet(0.0f, 0.0f, 0.0f);
	_cPos[0] = VTransform(v, m);

	// 重力処理
	for (int i = 1; i < Chain_Num; i++) {
		_cPos[i].y -= 12.0f;
		if (_cPos[i].y - 10.0f < 0.0f) {
			_cPos[i].y = 10.0f;
		}
	}
	_iPos.y -= 12.0f;
	if (_iPos.y < 0.0f) {
		_iPos.y = 0.0f;
	}

	// 鎖の位置調整
	//_length = 50.0f;
	for (int i = 0; i < Chain_Num - 1; i++) {
		VECTOR vNext = _cPos[i + 1];
		VECTOR vDelta = VSub(vNext, _cPos[i]);
		float distance = VSize(vDelta);
		float difference = _length - distance;

		float offsetX = (difference * vDelta.x / distance) * 0.9f;
		float offsetY = (difference * vDelta.y / distance) * 0.9f;
		float offsetZ = (difference * vDelta.z / distance) * 0.9f;

		//if (i != 0) {
		//	_cPos[i].x -= offsetX;
		//	_cPos[i].y -= offsetY;
		//	_cPos[i].z -= offsetZ;
		//}
		float mul = 1.0f;
		if (i == 0) mul = 2.0f;
		_cPos[i + 1].x += offsetX * mul;
		_cPos[i + 1].y += offsetY * mul;
		_cPos[i + 1].z += offsetZ * mul;
	}

	// 鉄球の位置
	_iPos = _cPos[Chain_Num - 1];
	MV1SetPosition(_iModelHandle, _iPos);

	VECTOR ForwardDir = VSub(_cPos[0], _iPos);
	if (VSize(ForwardDir) > 0.0f) {
		ForwardDir = VNorm(ForwardDir);
		Math::SetModelForward_RotationY(_iModelHandle, ForwardDir);
	}

	// アニメーションプロセス
	AnimProcess();
};

void LoadingChain::AnimProcess(){
	MV1SetAttachAnimTime(_iModelHandle, _animIndex, _playTime);

	_playTime += 1.0f;
	if (_animTotalTime < _playTime) {
		_playTime = 0.0f;
	}
};

void LoadingChain::SetPlayerModelHandle(int handle) {
	_playerModelHandle = handle;

	_socketNo[0] = MV1SearchFrame(_playerModelHandle, "chain1");
	_socketNo[1] = MV1SearchFrame(_playerModelHandle, "chain2");
	_socketNo[2] = MV1SearchFrame(_playerModelHandle, "chain3");
};

void LoadingChain::Render() {
	// 鎖の描画
	for (int i = 0; i < Chain_Num; i++) {
		// モデルに座標を反映させる
		MV1SetPosition(_cModelHandle, _cPos[i]);
		// モデルの描画
		MV1DrawModel(_cModelHandle);
	}

	// 鉄球の描画
	MV1SetPosition(_iModelHandle, _iPos);
	MV1DrawModel(_iModelHandle);
};