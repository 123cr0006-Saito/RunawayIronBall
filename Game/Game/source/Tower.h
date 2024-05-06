//----------------------------------------------------------------------
// @filename Tower.h
// @date: 2024/02/05
// @author: Morozumi Hiroya
// @explanation
// タワーオブジェクトの制御・描画を行うクラス
// 1つのタワーオブジェクトは、複数のTowerPartsクラスから構成される
//----------------------------------------------------------------------
#pragma once
#include "TowerParts.h"
#include "ObjectBase.h"

class Tower : public ObjectBase
{
public:
	Tower();
	virtual ~Tower();

	// 初期化処理
	// @param modelHandle: モデルハンドル（配列の要素数がタワーの段数となる）
	// @param startPos: 起点座標
	// @param rotation: モデルの回転値
	// @param scale: モデルの拡大率
	void Init(std::vector<int> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	// 更新処理
	void Process();
	// 描画処理
	void Render();

	// このタワーが使用中かどうかを取得
	bool GetUse() { return _use; }

	// 起点座標を取得
	VECTOR GetPos() { return _pos; }

	// タワーパーツの吹き飛ばし設定
	// @param vDir: 吹き飛ばす方向
	void SetBlast(VECTOR vDir);

	// タワーパーツが吹き飛ばし可能状態かどうかを取得
	bool GetCanBlast() { return _canBlast; }

	// タワー本体の当たり判定形状を取得
	Sphere GetCollision() { return _sphereCollision; }

	// デバッグ情報の表示
	void DrawDebugInfo();

protected:
	// このタワーが使用中かどうか（タワーパーツが残っているかどうか）
	bool _use;
	// 起点座標
	VECTOR _pos;

	// パーツの情報
	std::vector<TowerParts*> _towerParts;
	// パーツの初期の数
	int _partsNum;
	// 最下部のパーツのインデックス
	int _bottomIndex;
	
	// パーツの落下終了判定を行うかどうか
	bool _checkFallEnd;

	// プレイヤーからの攻撃判定を無効化中のカウント
	int _invincibleCnt;

	// パーツを吹き飛ばし可能かどうか
	bool _canBlast;

	// タワー本体の当たり判定形状
	// 1. キャラクターの体との当たり判定に使用
	// 2. プレイヤーからの攻撃判定に使用
	Sphere _sphereCollision;
};