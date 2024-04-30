//----------------------------------------------------------------------
// @filename IronBall.h
// @date: 2024/04/01
// @author: Morozumi Hiroya
// @explanation
// プレイヤーが持つ鉄球・鎖の制御・描画を行うクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "ObjectBase.h"
#include "Afterglow.h"
#include "AfterImage.h"

#define CHAIN_MAX 12

// 移動状態
enum IB_MOVE_STATE {
	// 追従状態
	FOLLOWING,
	// ソケットへの配置状態
	PUTTING_ON_SOCKET,
};
class IronBall : public ObjectBase
{
public:
	IronBall();
	virtual ~IronBall();

	void Init();
	// 親モデルの座標をもとに初期位置を設定
	void InitPosFromParent();
	void Process();

	//----------------------------------------------------------------------
	// 移動関連の処理

	// 移動処理
	void MoveProcess();
	// IB_MOVE_STATEに応じた処理を行う
	// 追従状態の処理
	void FollowingProcess();
	// ソケットへの配置状態の処理
	void PuttingOnSocketProcess();
	//----------------------------------------------------------------------

	// アニメーションの更新処理
	// 鉄球のアニメーションは1種類のみなのでAnimationManagerクラスを使用しない
	void AnimProcess();
	// 描画処理
	void Render();
	// 残像の描画
	void RenderAfterImage();

	VECTOR GetBallPosition() { return _iPos; }
	void SetBallPosition(VECTOR pos) { _iPos = pos; }

	VECTOR* GetBallPosPtr() { return &_iPos; }

	bool GetEnabledAttackCollision() { return _enabledAttackCollision; }
	void SetEnabledAttackCollision(bool state) { _enabledAttackCollision = state; }

	Sphere GetIBCollision() { return _ibAttackSphereCollision; }
	Sphere GetIBBodyCollision() { return _ibBodySphereCollision; }
	Capsule GetChainCollision() { return _chainCapsuleCollision; }

	// 鉄球の当たり判定を更新
	void UpdateIBCollision();
	// 鎖の当たり判定を更新
	void UpdateChainCollision();

	// 移動状態を設定する
	void SetMoveState(IB_MOVE_STATE state) { _moveState = state; }

	// 残像を表示するかどうかを設定
	void SetAddAfterImage(bool enable) { _addAfterImage = enable; }

	// サイトウが作成しました。
	void SetEnabledAfterGlow(bool enable);// 目の残光を表示するかどうかを設定

	//----------------------------------------------------------------------
	// このオブジェクトを保有している親オブジェクト関連の関数

	// 親オブジェクトのインスタンスのセッター・ゲッター
	void SetParentInstance(ObjectBase* parent) { _parent = parent; }
	ObjectBase* GetParentInstance() { return _parent; }
	// 親オブジェクトのモデルハンドルをセット
	void SetParentModelHandle(int handle);
	//----------------------------------------------------------------------

	// デバッグ情報の表示
	void DrawDebugInfo();


	bool UpdateLevel(float scale);//プレイヤーから取得した、レベルで攻撃力と拡大率を設定


private:
	XInput* _input;

	// 鉄球のモデルハンドル
	int _iModelHandle;
	// 鉄球の座標
	VECTOR _iPos;
	// 鉄球モデルの正面方向
	VECTOR _iForwardDir;
	// 鉄球モデルの初期拡大率
	VECTOR _ibDefaultScale;
	// 移動状態
	IB_MOVE_STATE _moveState;

	// 鎖のモデルハンドル
	int _cModelHandle;
	// 鎖の座標
	// 最後の要素は鉄球の座標とする
	VECTOR _cPos[CHAIN_MAX];
	// 鎖の輪同士の間隔
	float _lengthBetweenChains;

	// 鉄球・鎖のモデルモデルを配置する親モデルのソケット番号
	int _socketNo[3];

	//----------------------------------------------------------------------
	// 当たり判定関連

	// 攻撃判定が有効かどうか
	bool _enabledAttackCollision;
	// 鉄球部分の当たり判定形状（地面との当たり判定に使う）
	Sphere _ibBodySphereCollision;
	// 鉄球部分の攻撃当たり判定
	Sphere _ibAttackSphereCollision;
	// 鎖部分の当たり判定
	Capsule _chainCapsuleCollision;
	// 鎖部分の当たり判定をCollisionManagerに登録するためのCell
	Cell* _chainCell;
	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	// アニメーション関連

	// アタッチインデックス番号
	int _animIndex;
	// 総再生時間
	float _animTotalTime;
	// 現在の再生時間
	float _playTime;
	//----------------------------------------------------------------------

	// 残像クラス
	AfterImage* _afterImage;
	// 新しく残像を生成するかどうか
	bool _addAfterImage;

	// このオブジェクトを保有している親のオブジェクトへのポインタ
	ObjectBase* _parent;
	// 親オブジェクトのモデルハンドル
	int _parentModelHandle;

	
	std::vector<Afterglow*> _afterglowList;
};