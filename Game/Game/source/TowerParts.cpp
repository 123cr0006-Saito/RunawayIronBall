//----------------------------------------------------------------------
// @filename TowerParts.h
// ＠date: 2024/04/01
// ＠author: Morozumi Hiroya
// @explanation
// タワーパーツの制御を行うクラス
// 1つのタワーオブジェクトは、複数のTowerPartsクラスから構成される
//----------------------------------------------------------------------
#include "TowerParts.h"

namespace {
	constexpr float BLAST_SPEED = 60.0f;
	constexpr int BLAST_CNT_MAX = 180;
	constexpr float FALL_CNT_MAX = 30;
}

TowerParts::TowerParts() : ObjectBase::ObjectBase()
{
	_use = true;
	_useCollision = false;

	_blast = false;
	_blastDir = VGet(0.0f, 0.0f, 0.0f);
	_blastCnt = 0;

	_isFalling = false;
	_fallCnt = 0;
	_fallStartPos = VGet(0.0f, 0.0f, 0.0f);
	_fallEndPos = VGet(0.0f, 0.0f, 0.0f);

	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);

	_sphereCollision.centerPos = VGet(0.0f, 0.0f, 0.0f);
	_sphereCollision.r = 0.0f;
}

TowerParts::~TowerParts()
{
}

// 初期化処理
void TowerParts::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale)
{
	_modelHandle = modelHandle;
	_pos = startPos;

	MV1SetPosition(_modelHandle, _pos);
	MV1SetRotationXYZ(_modelHandle, VGet(0.0f, rotation.y, 0.0f));
	MV1SetScale(_modelHandle, scale);

	// 当たり判定の設定
	_sphereCollision.r = 250.0f;
	_cell->_objType = OBJ_TYPE::TWR_PRT;
}

// 更新処理
void TowerParts::Process()
{
	if (_use) {
		// 落下処理
		if (_isFalling) {
			FallProcess();
		}

		// 吹っ飛び処理
		if (_blast) {
			BlastOffProcess();
			// 当たり判定の更新
			UpdateCollision();
		}

		// モデルの座標を更新
		MV1SetPosition(_modelHandle, _pos);
	}
}

// 吹き飛び処理
void TowerParts::BlastOffProcess()
{
	_pos = VAdd(_pos, VScale(_blastDir, BLAST_SPEED));
	_blastCnt++;
	// 吹き飛びが終了したら、パーツを非使用状態にする
	if (_blastCnt > BLAST_CNT_MAX) {
		_use = false;
		// 当たり判定を無効にする
		_useCollision = false;
		_collisionManager->ReserveRemovementCell(_cell);
	}
}

// 落下処理
void TowerParts::FallProcess()
{
	float x = Easing::InQuint(_fallCnt, _fallStartPos.x, _fallEndPos.x, FALL_CNT_MAX);
	float y = Easing::InQuint(_fallCnt, _fallStartPos.y, _fallEndPos.y, FALL_CNT_MAX);
	float z = Easing::InQuint(_fallCnt, _fallStartPos.z, _fallEndPos.z, FALL_CNT_MAX);
	_pos = VGet(x, y, z);

	_fallCnt++;
	// 落下が終了したら
	if (_fallCnt > FALL_CNT_MAX) {
		_isFalling = false;
	}
}

// 描画処理
void TowerParts::Render()
{
	if (_use) {
		MV1SetPosition(_modelHandle, _pos);
		MV1DrawModel(_modelHandle);
	}
}

// 当たり判定の更新
void TowerParts::UpdateCollision()
{
	_sphereCollision.centerPos =_pos;
	_collisionManager->UpdateCell(_cell);
}

// デバッグ情報の表示
void TowerParts::DrawDebugInfo()
{
	if (_useCollision) {
		_sphereCollision.Render(COLOR_WHITE);
	}
}
