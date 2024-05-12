//----------------------------------------------------------------------
// @filename Tower.cpp
// @date: 2024/02/05
// @author: Morozumi Hiroya
// @explanation
// タワーオブジェクトの制御・描画を行うクラス
// 1つのタワーオブジェクトは、複数のTowerPartsクラスから構成される
//----------------------------------------------------------------------
#include "Tower.h"

namespace {
	// プレイヤーからの攻撃判定を無効化するフレーム数
	constexpr int INVINCIBLE_CNT_MAX = 30; 
}

Tower::Tower() : ObjectBase::ObjectBase()
{
	_use = true;
	_pos = VGet(0.0f, 0.0f, 0.0f);

	_towerParts.clear();
	_partsNum = 0;
	_bottomIndex = 0;

	_checkFallEnd = false;
	_invincibleCnt = -1;

	_canBlast = true;	

	_sphereCollision.centerPos = VGet(0.0f, 0.0f, 0.0f);
	_sphereCollision.r = 0.0f;
}

Tower::~Tower()
{
	for (auto itr = _towerParts.begin(); itr != _towerParts.end(); ++itr) {
		delete *itr;
	}
	_towerParts.clear();
}

// 初期化処理
// @param modelHandle: モデルハンドル（配列の要素数がタワーの段数となる）
// @param startPos: 起点座標
// @param rotation: モデルの回転値
// @param scale: モデルの拡大率
void Tower::Init(std::vector<int> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale)
{
	_pos = startPos;

	_partsNum = modelHandle.size();
	// タワーパーツを生成
	for (int i = 0; i < _partsNum; i++) {
		TowerParts* tp = NEW TowerParts();
		VECTOR tmpPos = VGet(0.0f, 0.0f, 0.0f);
		// 最下部のパーツは、startPosに配置する
		if (i == 0) {
			tmpPos = startPos;
		}
		// それ以外のパーツは、一つ下のパーツのソケット（ジョイント）座標に配置する
		else {
			VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
			MATRIX m = MV1GetFrameLocalWorldMatrix(_towerParts[i - 1]->GetModelHandle(), 1);
			tmpPos = VTransform(vOrigin, m);
		}
		
		tp->Init(modelHandle[i], tmpPos, rotation, scale);
		_towerParts.push_back(tp);
	}

	// タワー本体の当たり判定を設定
	_sphereCollision.centerPos = _pos;
	_sphereCollision.r = 250.0f;
	_cell->_objType = OBJ_TYPE::TWR;
	_collisionManager->UpdateCell(_cell);
}

// 更新処理
void Tower::Process()
{
	if (_use) {
		// パーツの落下終了判定
		if (_checkFallEnd) {
			// 残っている全てのパーツが落下終了したかどうかを判定する
			bool finishedFalling = true;
			for (int i = _bottomIndex; i < _partsNum; i++) {
				if(_towerParts[i]->GetUse() == false) continue;

				finishedFalling = finishedFalling && !(_towerParts[i]->GetIsFalling());
			}
			// 全てのパーツが落下終了したら
			if (finishedFalling) {
				// 落下終了判定をやめる
				_checkFallEnd = false;
				// パーツを吹き飛ばし可能状態にする
				_canBlast = true;
			}
		}

		// プレイヤーからの攻撃判定を無効化中の処理
		if (_invincibleCnt >= 0) {
			_invincibleCnt--;
			if (_invincibleCnt < 0) {
				_checkFallEnd = true;
			}
		}

	}

	// パーツの更新処理
	for (int i = 0; i < _partsNum; i++) {
		_towerParts[i]->Process();
	}
}

// 描画処理
void Tower::Render()
{
	for (int i = 0; i < _partsNum; i++) {
		_towerParts[i]->Render();
	}
}

// タワーパーツの吹き飛ばし設定
// @param vDir: 吹き飛ばす方向
void Tower::SetBlast(VECTOR vDir)
{
	// タワーが吹き飛ばし可能状態なら
	if (_canBlast) {
		_invincibleCnt = INVINCIBLE_CNT_MAX;
		_canBlast = false;

		for (int i = _bottomIndex; i < _partsNum; i++) {
			// 最下部のパーツのみ吹っ飛び処理
			if (i == _bottomIndex) {
				_towerParts[i]->SetBlast(VNorm(vDir));
			}
			// それ以外のパーツは落下処理
			else {
				// 一つ下のパーツの吹き飛ばし前の座標を、落下終了後の座標として設定
				_towerParts[i]->SetFalling(_towerParts[i - 1]->GetPos());
			}
		}
		_bottomIndex++;

		//パーツが残っていなかったら、タワー本体の当たり判定を無効化
		if (_bottomIndex >= _partsNum) {
			_use = false;
			_collisionManager->ReserveRemovementCell(_cell);
		}
	}
}

// デバッグ情報の表示
void Tower::DrawDebugInfo()
{
	if (_use) {
		_sphereCollision.Render(COLOR_GREEN);
	}
	for (int i = 0; i < _partsNum; i++) {
		_towerParts[i]->DrawDebugInfo();
	}
}
