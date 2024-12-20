//----------------------------------------------------------------------
// @filename IronBall.cpp
// @date: 2024/12/14
// @author: Morozumi Hiroya
// @explanation
// プレイヤーが持つ鉄球・鎖の制御・描画を行うクラス
//----------------------------------------------------------------------
#include "IronBall.h"

namespace {
	// 鉄球のSphereコリジョンの半径（地面との判定用）
	constexpr float IB_BODY_COLLISION_RADIUS = 50.0f;
	// 鉄球の攻撃Sphereコリジョンの半径
	constexpr float IB_ATTACK_COLLISION_RADIUS = 200.0f;
	// 鎖の攻撃Capsuleコリジョンの半径
	constexpr float CHAIN_COLLISION_RADIUS = 150.0f;
}

IronBall::IronBall()
{
	_cModelHandle = -1;
	_iModelHandle = -1;
	_parentModelHandle = -1;
	_input = nullptr;
	_iForwardDir = VGet(0, 0, 0);

	for(int i = 0; i < CHAIN_MAX; i++) {
		_cPos[i] = VGet(0, 0, 0);
	}

	_iPos = VGet(0, 0, 0);
	_ibDefaultScale = VGet(0, 0, 0);

	_ibBodySphereCollision.centerPos = VGet(0, 0, 0);
	_ibBodySphereCollision.r = 0;
	_ibAttackSphereCollision.centerPos = VGet(0, 0, 0);
	_ibAttackSphereCollision.r = 0;

	_chainCapsuleCollision.up_pos = VGet(0, 0, 0);
	_chainCapsuleCollision.down_pos = VGet(0, 0, 0);
	_chainCapsuleCollision.r = 0;

	_chainCell = nullptr;

	_animIndex = 0;
	_animTotalTime = 0;
	_playTime = 0;

	_afterImage = nullptr;
	_addAfterImage = false;

	_lengthBetweenChains = 0;
	_moveState = IB_MOVE_STATE::FOLLOWING;
	_enabledAttackCollision = false;
	_socketNo[0] = -1;
	_socketNo[1] = -1;
	_socketNo[2] = -1;
	_parent = nullptr;
}

IronBall::~IronBall()
{
	_input = nullptr;
	_parent = nullptr;
	delete _cell; _cell = nullptr;
	delete _chainCell; _chainCell = nullptr;
	delete _afterImage; _afterImage = nullptr;
	for (auto list : _afterglowList) {
		delete list;
	}
}

void IronBall::Init() {
	_input = XInput::GetInstance();
	_cModelHandle = ResourceServer::MV1LoadModel("Chain", "res/Chain/Cg_Chain.mv1");
	_cPos[0] = VGet(0.0f, 0.0f, 0.0f);
	MV1SetPosition(_cModelHandle, _cPos[0]);
	MV1SetScale(_cModelHandle, VGet(0.5f, 0.5f, 0.5f));

	for (int i = 1; i < CHAIN_MAX; i++) {
		_cPos[i] = VAdd(_cPos[i - 1], VGet(0.0f, 0.0f, -100.0f));
	}

	_iModelHandle = ResourceServer::MV1LoadModel("IronBall", "res/Character/Cg_Iron_Ball/Cg_Iron_Ball.mv1");
	_iPos = VAdd(_cPos[CHAIN_MAX - 1], VGet(0.0f, 10.0f, 0.0f));
	_ibDefaultScale = VGet(2.5f, 2.5f, 2.5f);
	MV1SetScale(_iModelHandle, _ibDefaultScale);
	MV1SetPosition(_iModelHandle, _iPos);

	_ibBodySphereCollision.centerPos = _iPos;
	_ibBodySphereCollision.r = IB_BODY_COLLISION_RADIUS;
	_ibAttackSphereCollision.centerPos = _iPos;
	_ibAttackSphereCollision.r = IB_ATTACK_COLLISION_RADIUS;

	_chainCapsuleCollision.up_pos = _cPos[0];
	_chainCapsuleCollision.down_pos = _cPos[CHAIN_MAX - 1];
	_chainCapsuleCollision.r = CHAIN_COLLISION_RADIUS;


	_animIndex = MV1AttachAnim(_iModelHandle, 0);
	_animTotalTime = MV1GetAnimTotalTime(_iModelHandle, _animIndex);
	_playTime = 0.0f;


	_iForwardDir = VGet(0, 0, 0);

	_lengthBetweenChains = 50.0f;

	_moveState = IB_MOVE_STATE::FOLLOWING;

	_enabledAttackCollision = false;

	_cell->_objType = OBJ_TYPE::PL_IB;
	
	_chainCell = NEW Cell();
	_chainCell->_obj = this;
	_chainCell->_objType = OBJ_TYPE::PL_IB_CHAIN;


	_afterImage = NEW AfterImage();
	_afterImage->Init(_iModelHandle, "IB_AfterImage", "res/Character/Cg_Iron_Ball/Cg_Iron_Ball.mv1", 10, 10);


	int afterglow = MV1SearchFrame(_iModelHandle, "left_eye02");
	_afterglowList.push_back(NEW Afterglow(_iModelHandle, afterglow, 10, LoadGraph("res/Effect/afterglow.png"), 20));
	afterglow = MV1SearchFrame(_iModelHandle, "right_eye02");
	_afterglowList.push_back(NEW Afterglow(_iModelHandle, afterglow, 10, LoadGraph("res/Effect/afterglow.png"), 20));
}

// 親モデルの座標をもとに初期位置を設定
void IronBall::InitPosFromParent()
{
	MATRIX m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[0]);
	VECTOR v = VGet(0.0f, 0.0f, 0.0f);
	_cPos[0] = VTransform(v, m);

	for (int i = 1; i < CHAIN_MAX; i++) {
		_cPos[i] = VAdd(_cPos[i - 1], VGet(0.0f, 0.0f, _lengthBetweenChains));
	}
	_iPos = _cPos[CHAIN_MAX - 1];
}


void IronBall::Process() {
	// 親モデルの手元に配置する鎖の座標を設定
	MATRIX m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[0]);
	VECTOR v = VGet(0.0f, 0.0f, 0.0f);
	_cPos[0] = VTransform(v, m);

	// 移動処理
	MoveProcess();

	// 鎖が地面に埋まらないようにする
	for (int i = 1; i < CHAIN_MAX; i++) {
		if (_cPos[i].y - 10.0f < 0.0f) {
			_cPos[i].y = 10.0f;
		}
	}

	// 鉄球の座標を更新
	_iPos = _cPos[CHAIN_MAX - 1];
	// 鉄球が地面に埋まらないようにする
	if (_iPos.y - _ibBodySphereCollision.r < 0.0f) {
		_iPos.y = 0.0f + _ibBodySphereCollision.r;
	}

	// 鉄球の正面方向を設定
	// 鉄球から見た鎖と親モデルの連結点の方向を向くようにする
	_iForwardDir = VSub(_cPos[0], _iPos);
	if (VSquareSize(_iForwardDir) > 0.0f) {
		// 追従状態出ない時は、正面方向を逆にする
		if (_moveState != IB_MOVE_STATE::FOLLOWING) _iForwardDir = VScale(_iForwardDir, -1.0f);
		Math::SetModelForward_RotationY(_iModelHandle, _iForwardDir);
	}



	// 当たり判定の更新
	UpdateIBCollision();
	_collisionManager->UpdateCell(_cell);

	// 攻撃判定が有効なら鎖の当たり判定を更新
	if (_enabledAttackCollision) {
		UpdateChainCollision();
		_collisionManager->UpdateCell(_chainCell);
	}
	// 無効なら鎖の当たり判定を削除
	else {
		if (_chainCell->_segment != nullptr) {
			_collisionManager->ReserveRemovementCell(_chainCell);
		}
	}

	// アニメーションの更新
	AnimProcess();

	// 残像の処理
	_afterImage->Process();
	// 新しく残像を生成する場合
	if (_addAfterImage) {
		_afterImage->AddAfterImage(0, _playTime);
	}

	// 残光の処理
	for (auto list : _afterglowList) {
		list->Process();
	}
}

// 移動処理
void IronBall::MoveProcess()
{
	switch (_moveState)
	{
	case FOLLOWING:
		FollowingProcess();
		break;
	case PUTTING_ON_SOCKET:
		PuttingOnSocketProcess();
		break;
	}
}

// 追従状態の処理
void IronBall::FollowingProcess()
{
	// 重力処理
	for (int i = 1; i < CHAIN_MAX; i++) {
		_cPos[i].y -= 12.0f;
		if (_cPos[i].y - 10.0f < 0.0f) {
			_cPos[i].y = 10.0f;
		}
	}
	_iPos.y -= 12.0f;
	if (_iPos.y < 0.0f) {
		_iPos.y = 0.0f;
	}

	// 鎖同士の間隔を一定に保つ
	for (int i = 0; i < CHAIN_MAX - 1; i++) {
		VECTOR vNext = _cPos[i + 1];
		VECTOR vDelta = VSub(vNext, _cPos[i]);
		float distance = VSize(vDelta);
		float difference = _lengthBetweenChains - distance;

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
}

// ソケットへの配置状態の処理
void IronBall::PuttingOnSocketProcess()
{
	// 各ソケットへの配置
	// 配置位置はアニメーションで設定されている
	{
		VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
		MATRIX m = MGetIdent();

		// 0番目（親モデルの手元の位置）
		m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[0]);
		_cPos[0] = VTransform(vOrigin, m);

		// 1番目（0番目につながる鎖の位置）
		m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[1]);
		_cPos[1] = VTransform(vOrigin, m);

		// 2番目（鉄球の位置）
		m = MV1GetFrameLocalWorldMatrix(_parentModelHandle, _socketNo[2]);
		_cPos[CHAIN_MAX - 1] = VTransform(vOrigin, m);
	}

	// 残りの鎖の位置はプログラムで補間する

	// 0番目の鎖から見た1番目の鎖を配置する方向
	VECTOR vBase = VSub(_cPos[1], _cPos[0]);
	// 0番目の鎖から見た2番目（鉄球）を配置する方向
	VECTOR vTarget = VSub(_cPos[CHAIN_MAX - 1], _cPos[0]);

	// 2つのベクトルの角度を求める
	float rad = Math::CalcVectorAngle(vBase, vTarget);
	// 0番目と2番目（鉄球）の距離を計算する
	float dist = VSize(vTarget);

	vBase = VNorm(vBase);

	// vBaseとvTargetの外積を求め、それを回転の軸とする
	VECTOR vCross = VCross(vBase, vTarget);

	// 鎖の位置を補間
	const float chainNum = CHAIN_MAX - 1;
	for (int i = 1; i < CHAIN_MAX; i++) {
		// 割合i/chainNumに応じて、位置の補間を行う
		float rate = (float)(i) / chainNum;

		// 0番目の鎖からの移動量を計算
		VECTOR vTmp = VScale(vBase, dist * rate);
		// 上の行で計算した位置をvCrossを軸に回転させる（回転前は0番目の鎖からvBase方向に一列に鎖が並んでいる状態）
		MATRIX mRot = MGetRotAxis(vCross, rad * rate);
		vTmp = VTransform(vTmp, mRot);
		_cPos[i] = VTransform(vTmp, MGetTranslate(_cPos[0]));

		if (_cPos[i].y < 0.0f) {
			_cPos[i].y = 0.0f;
		}
	}
}

// アニメーションの更新処理
// 鉄球のアニメーションは1種類のみなのでAnimationManagerクラスを使用しない
void IronBall::AnimProcess()
{
	MV1SetAttachAnimTime(_iModelHandle, _animIndex, _playTime);

	_playTime += 1.0f;	
	if (_animTotalTime < _playTime) {
		_playTime = 0.0f;
	}
}

// 描画処理
void IronBall::Render()
{
	// 残光の描画
	for (auto list : _afterglowList) {
		list->Render();
	}
	// 鎖の描画
	for (int i = 0; i < CHAIN_MAX - 1; i++) { //  最後の要素は鉄球の座標となるため, i < CHAIN_MAX - 1
		// モデルに座標を反映させる
		MV1SetPosition(_cModelHandle, _cPos[i]);
		// モデルの描画
		MV1DrawModel(_cModelHandle);
	}

	// 鉄球の描画
	MV1SetPosition(_iModelHandle, _iPos);
	MV1DrawModel(_iModelHandle);
}

// 残像の描画
void IronBall::RenderAfterImage()
{
	// 残像の描画
	_afterImage->Render();
}

// 鉄球の当たり判定を更新
void IronBall::UpdateIBCollision()
{
	_ibBodySphereCollision.centerPos = _iPos;
	_ibAttackSphereCollision.centerPos = _iPos;
}

// 鎖の当たり判定を更新
void IronBall::UpdateChainCollision()
{
	_chainCapsuleCollision.up_pos = _cPos[0];
	_chainCapsuleCollision.down_pos = _cPos[CHAIN_MAX - 1];
}

// 目の残光を表示するかどうかを設定
// サイトウが作成しました。
void IronBall::SetEnabledAfterGlow(bool enable) {
	for (auto list : _afterglowList) {
		list->SetUpdate(enable);
	}
}

// 親オブジェクトのモデルハンドルをセット
void IronBall::SetParentModelHandle(int handle)
{
	_parentModelHandle = handle;

	// 鉄球・鎖のモデルを配置する親モデルのソケット番号を取得
	_socketNo[0] = MV1SearchFrame(_parentModelHandle, "chain1");
	_socketNo[1] = MV1SearchFrame(_parentModelHandle, "chain2");
	_socketNo[2] = MV1SearchFrame(_parentModelHandle, "chain3");
}

// デバッグ情報の表示
void IronBall::DrawDebugInfo() {
	unsigned int color = _enabledAttackCollision ? COLOR_RED : COLOR_WHITE;
	_ibBodySphereCollision.Render(COLOR_GREEN);
	_ibAttackSphereCollision.Render(color);
	_chainCapsuleCollision.Render(color);

	//int x = 0;
	//int y = 0;
	//int line = 0;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_pos: x %3.2f, y %3.2f, z  %3.2f", _pos.x, _pos.y, _pos.z); line++;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_dir: x %3.2f, y %3.2f, z  %3.2f", _dir.x, _dir.y, _dir.z); line++;
	//DrawFormatString(x, y + line * 16, COLOR_WHITE, "_speed %3.2f", _speed); line++;
}

bool IronBall::UpdateLevel(float scale)
{
	MV1SetScale(_iModelHandle, VScale(_ibDefaultScale, scale));
	_ibBodySphereCollision.r = IB_BODY_COLLISION_RADIUS * scale;
	_ibAttackSphereCollision.r = IB_ATTACK_COLLISION_RADIUS * scale;
	return true;
}
