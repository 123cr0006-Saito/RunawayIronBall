//----------------------------------------------------------------------
// @filename TowerParts.h
// @date: 2024/04/01
// @author: Morozumi Hiroya
// @explanation
// タワーパーツの制御を行うクラス
// 1つのタワーオブジェクトは、複数のTowerPartsクラスから構成される
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "ObjectBase.h"

class TowerParts : public ObjectBase
{
public:

	TowerParts();
	virtual ~TowerParts();

	// 初期化処理
	void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	// 更新処理
	void Process();
	// 描画処理
	void Render();

	// 当たり判定の更新
	void UpdateCollision();



	void SetUse(bool use) { _use = use; }
	bool GetUse() { return _use; }

	int GetModelHandle() { return _modelHandle; }

	VECTOR GetPos() { return _pos; }

	// 当たり判定の使用設定
	void SetUseCollision(bool useCollision) {
		_useCollision = useCollision;
		if (_useCollision) {
			UpdateCollision();
		}
	}

	// 吹き飛び処理の設定
	void SetBlast(VECTOR vDir) {
		SetUseCollision(true);
		_blast = true;
		_blastDir = vDir;

		_isFalling = false;
	}

	bool GetIsBlast() { return _blast; }

	// 落下処理の設定
	void SetFalling(VECTOR endPos) {
		_isFalling = true;
		_fallCnt = 0;
		_fallStartPos = _pos;
		_fallEndPos = endPos;
	}

	bool GetIsFalling() { return _isFalling; }

	Sphere GetCollision() { return _sphereCollision; }


	// デバッグ情報の表示
	void DrawDebugInfo();

protected:
	// 吹き飛び処理
	void BlastOffProcess();
	// 落下処理
	void FallProcess();

	// このタワーパーツが使用されているかどうか
	bool _use;

	// モデルハンドル
	int _modelHandle;
	// 座標
	VECTOR _pos;

	bool _useCollision;

	//--------------------------------------------------
	// 吹き飛び処理

	// 吹き飛び中かどうか
	bool _blast;
	// 吹き飛び方向
	VECTOR _blastDir;
	// 吹き飛びカウント
	int _blastCnt;
	//--------------------------------------------------

	//--------------------------------------------------
	// 落下処理

	// 落下中かどうか
	bool _isFalling;
	// 落下中のカウント
	int _fallCnt;
	// 落下開始位置
	VECTOR _fallStartPos;
	// 落下終了位置
	VECTOR _fallEndPos;
	// -------------------------------------------------


	// 当たり判定形状
	// 吹き飛び状態での、敵や建物との当たり判定に使用する
	Sphere _sphereCollision;
};