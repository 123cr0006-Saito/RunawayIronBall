//----------------------------------------------------------------------
// @filename BossIronBall.h
// @date: 2024/03/20
// @author: Morozumi Hiroya
// @explanation
// ボス鉄球の制御を行うクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "Player.h"
#include "AfterImage.h"

class BossIronBall
{
public:
	BossIronBall();
	~BossIronBall();

	// モデルの読み込み
	void LoadModel();
	// 初期化処理
	void Init(VECTOR* stakePos);
	// 更新処理
	void Process();
	// 描画処理
	void Render();
	// 残像の描画処理
	void RenderAfterImage();

	// ガラス状態にする
	void ChangeGlass();
	// ガラス化状態かどうかを取得する
	bool GetIsGlass() { return _isGlass; }

	// ボス鉄球の座標を取得
	VECTOR GetPosition() { return _ibPos; }
	// ボス鉄球の座標を設定
	void SetPosition(VECTOR pos) { _ibPos = pos; }

	// 当たり判定の更新
	void UpdateIBCollision();
	// ボス鉄球の当たり判定を取得する
	Sphere GetIBCollision() { return _ibSphereCol; }

	// 鉄球の当たり判定を更新
	void UpdateChainCollision();
	// 鎖の当たり判定を取得する
	Capsule GetChainCollision() { return _chainCapsuleCol; }

	// 回転攻撃中かどうかを取得する
	bool GetIsRotationAttack() { return _isRotationAttack; }

	// ステージ上にいるかどうかを設定する
	void SetOnStage(bool isOnStage);
	// ステージ上にいるかどうかを取得する
	bool GetOnStage() { return _isOnStage; }

	// 無敵状態かどうかを取得する
	bool GetIsInvincible() { return _isInvincible; }

	// 当たり判定を行うかどうかを取得する
	bool GetUseCollision() { return _useCollision; }

	// 強化状態かどうかを取得する
	bool GetIsEnhanced() { return _isEnhanced; }
	// 強化状態に移行する
	void SetEnhanced() { _changeEnhanced = true; }

	// ハードノックバック状態かどうかを取得する
	bool CheckHardKnockBack() { return _ibState == IB_STATE::HARD_KNOCK_BACK; }

	// 鎖に当たったかどうかを取得する
	bool GetHitStake() { return _isHitStake; }
	// 鎖に当たったかどうかを設定する
	void SetHitStake(bool isHit) { _isHitStake = isHit; }

	// 杭が破壊状態かどうかを設定する
	void SetISStakeBroken(bool isBroken) { _isStakeBroken = isBroken; }

	// プレイヤーが鉄球の探索範囲内にいるかどうかを取得する
	// // 索敵範囲内にいる場合は索敵範囲のインデックスを返す 
	// @return: -1: 探索範囲内にいない, 0: 探索範囲0にいる, 1: 探索範囲1にいる
	int CheckPlayerInSearchRange();

	// ノックバックの設定を行う
	// @param vDir: ノックバック方向
	// @param speed: ノックバック速度
	void SetKnockBack(VECTOR vDir, float speed);
	// ノックバック状態かどうかを取得する
	bool CheckKnockBack() { return _isKnockBack; }

	// ステージの半径を設定する
	void SetStageRadius(float radius) { _stageRadius = radius; }

	// デバッグ情報の表示
	void DrawDebugInfo();


private:
	// 鉄球モデルの向きを更新する
	void UpdateModelRotation();

	// 行動状態を更新する
	void CheckState();

	// 強化状態に移行する
	// 強化状態に遷移が可能なタイミングに呼び出し、初めて_changeEnhancedがtrueになったときに強化状態に移行する（この場合は_isEnhancedは必ずfalse）
	void CheckChangeEnhanced();

	// フェーズのリセット
	// 各行動状態は複数のフェーズを持ち、行動状態が変わるとフェーズがリセットされる
	void ResetPhase();


	// 待機状態の処理
	void IdleProcess();
	// 待機状態に遷移する
	void SetIdle();

	// 硬直状態の処理
	void StiffenProcess();
	// 硬直状態に遷移する
	void SetStiffen(int cnt, bool isInvincible = false, bool playSound = true);


	// 突進攻撃の処理
	void RushProcess();
	// 突進攻撃に遷移する
	void SetRush();

	// 落下攻撃の処理
	void DropProcess();
	// 落下攻撃に遷移する
	void SetDrop();

	// 回転攻撃の処理
	void RotationProcess();
	// 回転攻撃に遷移する
	void SetRotation();

	// ノックバック処理
	void KnockBackProcess();

	// ハードノックバック処理
	void HardKnockBackProcess();

	// 重力処理
	void GravityProcess();

	// 鎖の更新処理
	void ChainProcess();

	// アニメーションの更新処理
	void AnimationProcess();

private:
	// 鉄球のモデルハンドル
	// 現在の状態に応じて変更される（_ibModelHandleArrayのどちらかを代入する）
	int _ibModelHandle;
	// 鉄球のモデルのハンドル配列
	// 0: 通常時, 1: ガラス状態時
	std::array<int, 2> _ibModelHandleArray;
	// ガラス化しているかどうか
	bool _isGlass;

	// 鉄球の座標
	VECTOR _ibPos;
	// 鉄球のモデルの向き
	VECTOR _ibModelForwardDir;
	// 鉄球のモデルを次に向かせる向き（現在の向きから数フレームかけて向きを補間する）
	VECTOR _ibModelNextForwardDir;

	// 鉄球モデルの向きの状態
	// _ibNextForwardDirの向きを更新するかどうか
	enum class IB_MODEL_DIR {
		PLAYER,						// プレイヤーの方向
		PLAYER_REVERSE,		// プレイヤーの逆方向
		STAKE,						// 杭の方向
		STAKE_REVERSE,		// 杭の逆方向
		NOT_UPDATE,			// _ibModelNextForwardDirを更新しない（最後に設定された値を保持する）
	} _ibModelDirState;

	// 鉄球の当たり判定
	Sphere _ibSphereCol;
	// 鎖の当たり判定
	Capsule _chainCapsuleCol;
	// 回転攻撃中かどうか
	bool _isRotationAttack;

	// ステージの中にいるかどうか
	bool _isOnStage;

	// 無敵状態かどうか
	bool _isInvincible;
	// 当たり判定を行うかどうか
	bool _useCollision;
	// 強化状態かどうか
	bool _isEnhanced;
	// 強化状態に切り替えるかどうか
	// 杭のHPが半分以下になったときにtrueにする
	// 一度trueになったらfalseには戻らない
	bool _changeEnhanced;
	// 強化状態での攻撃の回数
	int _enhancedAttackCnt;

	// 行動状態
	enum class IB_STATE {
		IDLE,							// 待機
		STIFFEN,					// 硬直
		ATTACK_RUSH,			// 突進攻撃
		ATTACK_DROP,			// 落下攻撃
		ATTACK_ROTATION,	// 回転攻撃
		KNOCK_BACK,			// ノックバック
		HARD_KNOCK_BACK,	// ハードノックバック（硬直時にプレイヤーの攻撃が当たった場合の状態, 杭まで直接飛んでいき、杭にダメージが入る）
	} _ibState;

	// 各行動状態は複数のフェーズを持ち、行動状態が変わるとフェーズがリセットされる
	// 各行動状態でのフェーズ番号
	int _phase;
	// フェーズ内の経過フレーム数
	int _phaseCnt;

	// 移動前の座標（移動の補間に使用する）
	VECTOR _posBeforeMoving;
	// 移動後の座標（移動の補間に使用する）
	VECTOR _targetPos;

	// 鎖に当たったかどうか
	bool _isHitStake;

	// 待機状態のフレーム数
	// 以下の範囲でランダムに設定される
	// IDLE_INTERVAL_BASE + (rand() % IDLE_INTERVAL_ADD_MAX)
	int _ibIdleCnt;

	// 移動方向
	VECTOR _ibMoveDir;

	// 硬直状態のフレーム数
	int _ibStiffenCnt;



	// 回転攻撃
	//  基準方向ベクトル
	VECTOR _rotBaseDir;
	// 角速度
	float _rotAngularVelocity;
	// _rotBaseDirを基準とした回転角度
	float _rotAngle;
	// 回転の半径
	float _rotRadius;


	// ノックバック状態かどうか
	bool _isKnockBack;
	// ノックバック方向
	VECTOR _knockBackDir;
	// ノックバック速度
	float _knockBackSpeed;
	// ノックバックを行うの残りフレーム数
	int _knockBackCnt;

	float _gravity;

	// 鎖のモデルハンドル
	int _chainModelHandle;
	// 鎖の座標
	std::vector<VECTOR> _chainPos;
	// 鎖同士の間隔
	float _chainDistance;

	// 杭の座標
	VECTOR* _stakePos;
	// 杭が破壊されたかどうか
	bool _isStakeBroken;

	// 原点から杭の座標への平行移動行列
	MATRIX _mStakePos;
	// _mStakePosの逆行列
	MATRIX _mStakePosInv;


	// アニメーション関連
	// アニメーションのインデックス
	int _animIndex;
	// アニメーションの合計時間
	float _animTotalTime;
	// 再生時間
	float _playTime;

	// 残像クラス
	AfterImage* _afterImage;
	// 新しく残像を生成するかどうか
	bool _addAfterImage;

	// SEを再生するかどうか
	bool _playSound;

	// ステージの半径
	float _stageRadius;

	// プレイヤーのインスタンス
	Player* _player;

};