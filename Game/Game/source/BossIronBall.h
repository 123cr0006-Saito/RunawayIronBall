#pragma once
#include "appframe.h"
#include "Player.h"

class BossIronBall
{
public:
	BossIronBall();
	~BossIronBall();

	void LoadModel();
	void Init(VECTOR* stakePos);

	void Process();
	void Render();

	VECTOR GetPosition() { return _ibPos; }
	void SetPosition(VECTOR pos) { _ibPos = pos; }


	void UpdateIBCollision();
	Sphere GetIBCollision() { return _ibSphereCol; }

	bool GetIsInvincible() { return _isInvincible; }
	bool GetUseCollision() { return _useCollision; }

	bool GetIsEnhanced() { return _isEnhanced; }
	void SetEnhanced() { _changeEnhanced = true; }


	bool CheckHardKnockBack() { return _ibState == IB_STATE::HARD_KNOCK_BACK; }

	void SetHitStake(bool isHit) { _isHitStake = isHit; }
	bool GetHitStake() { return _isHitStake; }

	void SetISStakeBroken(bool isBroken) { _isStakeBroken = isBroken; }


	int CheckPlayerInSearchRange();

	void SetKnockBack(VECTOR vDir, float speed);
	bool CheckKnockBack() { return _isKnockBack; }

	// デバッグ情報の表示
	void DrawDebugInfo();


private:
	// 鉄球モデルの向きを更新する
	void UpdateModelRotation();

	void CheckState();

	// 強化状態に移行する
	// 強化状態に遷移が可能なタイミングに呼び出し、初めて_changeEnhancedがtrueになったときに強化状態に移行する（この場合は_isEnhancedは必ずfalse）
	void CheckChangeEnhanced();

	void ResetPhase();


	// 待機状態の処理
	void IdleProcess();
	void SetIdle();

	// 硬直状態の処理
	void StiffenProcess();
	// 硬直状態に遷移する
	void SetStiffen(int cnt, bool isInvincible = false);


	// 突進攻撃
	void RushProcess();
	void SetRush();

	// 落下攻撃
	void DropProcess();
	void SetDrop();

	// 回転攻撃
	void RotationProcess();
	void SetRotation();

	// ノックバック処理
	void KnockBackProcess();

	// ハードノックバック処理
	void HardKnockBackProcess();

	// 
	void GravityProcess();

	void ChainProcess();

	// アニメーションの再生時間を更新
	void AnimationProcess();

private:
	// 鉄球のモデルハンドル
	int _ibModelHandle;
	// 鉄球の座標
	VECTOR _ibPos;
	// 鉄球のモデルの向き
	VECTOR _ibModelForwardDir;
	// 鉄球のモデルを次に向かせる向き（現在の向きから数フレームかけて向きを補間する）
	VECTOR _ibModelNextForwardDir;

	enum class IB_MODEL_DIR {
		PLAYER,		// プレイヤーの方向
		PLAYER_REVERSE,		// プレイヤーの逆方向
		STAKE,		// 杭の方向
		STAKE_REVERSE,		// 杭の逆方向
		NOT_UPDATE,		// 更新しない
	} _ibModelDirState;

	// 鉄球の当たり判定
	Sphere _ibSphereCol;
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

	enum class IB_STATE {
		IDLE,							// 待機
		STIFFEN,					// 硬直
		ATTACK_RUSH,			// 突進攻撃
		ATTACK_DROP,			// 落下攻撃
		ATTACK_ROTATION,	// 回転攻撃
		KNOCK_BACK,			// ノックバック
		HARD_KNOCK_BACK,	// ハードノックバック（硬直時にプレイヤーの攻撃が当たった場合の状態, 杭まで直接飛んでいく）
	} _ibState;

	// 各ステート内でのフェーズ番号
	int _phase;
	int _phaseCnt;

	VECTOR _posBeforeMoving;
	VECTOR _targetPos;

	bool _isHitStake;

	//
	int _ibIdleCnt;
	VECTOR _ibMoveDir;

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



	Player* _player;


	// デバッグ用
	// 特定のアクションのフレーム数
	int debugFrame = 30;
	int debugFrameMax = 30;

	float debugValue = 100.0f;
	float debugValueMax = 100.0f;
};