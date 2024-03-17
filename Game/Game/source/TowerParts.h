#pragma once
#include "appframe.h"
#include "ObjectBase.h"

class TowerParts : public ObjectBase
{
public:
	friend class Tower;

	TowerParts();
	virtual ~TowerParts();

	// 初期化処理
	void Init(int modelHandle, VECTOR startPos);
	// 更新処理
	void Process();
	// 描画処理
	void Render();

	// 当たり判定の更新
	void UpdateCollision();



	void SetUse(bool use) {
		_use = use;
	}

	bool GetUse() {
		return _use;
	}

	VECTOR GetPos() {
		return _pos;
	}

	void SetUseCollision(bool useCollision) {
		_useCollision = useCollision;
		if (_useCollision) {
			UpdateCollision();
		}
	}

	void SetBlast(VECTOR vDir) {
		SetUseCollision(true);
		_blast = true;
		_blastDir = vDir;

		_isFalling = false;
	}

	bool GetIsBlast() { return _blast; }

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
	// 吹っ飛び処理
	void BlastOffProcess();
	// 落下処理
	void FallProcess();


protected:
	bool _use;

	bool _useCollision;

	bool _blast;
	VECTOR _blastDir;
	int _blastCnt;

	bool _isFalling;
	int _fallCnt;
	VECTOR _fallStartPos;
	VECTOR _fallEndPos;


	int _modelHandle;
	VECTOR _pos;
	VECTOR _vRot;

	Sphere _sphereCollision;
};