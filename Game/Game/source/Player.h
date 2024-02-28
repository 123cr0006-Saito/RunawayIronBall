#pragma once
#include "appframe.h"
#include "CharacterBase.h"
#include "bone.h"
#include "myJson.h"

#include "Chain.h"

#include "AnimationManager.h"
#include "AnimationItem.h"

#include "FrameData.h"

#include "ModelColor.h"

enum IB_MOVE_STATE {
	FOLLOWING,
	PUTTING_ON_SOCKET,
	INTERPOLATION,
};

class Player : public CharacterBase
{
private:
	enum class ANIM_STATE {
		IDLE,	
		IDLE_TIRED,
		WALK,
		WALK_TIRED,
		RUN,

		HORISONTAL_SWING_01,
		HORISONTAL_SWING_02,
		HORISONTAL_SWING_03,

		MANYTIME_SWING,

		TO_ROTATION_SWING,
		ROTATION_SWING,

		IDLE_FIGHTING,

		LONG_JUMP_AIR,
		LONG_JUMP_NOSEDIVE,
		LONG_JUMP_LANDING,

		GAMEOVER,

		AVOIDANCE,
		HIT,
	};

public:
	Player(int modelHandle, VECTOR pos);
	~Player() override;

	bool Process(float camAngleY);
	bool AnimationProcess();
	bool BlastOffProcess();
	bool Render() override;


	int GetHP() { return _hp; }
	bool GetIsInvincible() { return _isInvincible; }
	// 無敵状態の更新
	void ChangeIsInvincible(bool b, int frame);

	void SetDamage();


	// キャラモデルの点滅処理
	void FlickerProcess();



	float GetStamina() { return _stamina; }
	float GetStaminaMax() { return _staminaMax; }
	float GetStaminaRate() { return _stamina / _staminaMax; }





	void SetBone();//齋藤が作った関数です。 boneのフレームを探すために使用する関数です。後でjsonでの読み込みにするかもしれません。
	//↓齋藤が作った関数です。どこにjson読み込みをどこに書けばよいのかわからなかったので、コンストラクタの次に呼び出す関数として実装しました。
	void SetNextExp(std::string FileName);//経験値データの読み込み
	bool  UpdateExp();//経験値が越えていた時、レベルを上げる。
	int GetNowLevel() { return _nowLevel; };
	void SetExp(int getExp) { _nowExp += getExp; };
	//経験値UIで使用しています。
	int GetNowExp() { return _nowExp; }
	int GetNextExp() { return _nextLevel[_nowLevel]; }


	void SetPowerScale(std::string FileName);//ファイル読み込みでレベルに合わせた攻撃力と拡大率を取得
	bool UpdateLevel();// レベルアップ時に攻撃力と拡大率を設定
	int GetPower() { return _power; }//ノックバック用の力を返します。



	void UpdateBone();
	void UpdateCollision();

	Capsule GetCollision() { return _capsuleCollision; };
	Sphere GetIBCollision() { return _chain->GetCollision(); };

	void SetBlastOffPower(VECTOR dir, float power) { _blastOffDir = dir; _blastOffPower = power; };



	VECTOR GetRightHandPos();

	VECTOR* GetIBPosPtr() { return &_chain->GetBallPosition(); }
	IB_MOVE_STATE GetIBMoveState() { return _ibMoveState; }


	bool GetAttackState() { return _isAttackState; }
	bool GetEnabledIBAttackCollision() { return _enabledIBAttackCollision; }

	// フレームデータのコマンドをチェックする
	void CheckFrameDataCommand();

	static Player* GetInstance() { return _instance; }

	void DrawDebugInfo();
private:
	// 入力情報
	XInput* _input;
	// Lスティックの入力方向
	// Lスティック入力があった場合に更新する
	VECTOR _stickDir;

	/* ステータス関連 */
	// HP
	int _hp;
	// 無敵かどうか
	bool _isInvincible;
	// 残りの無敵時間
	int _invincibleRemainingCnt;

	// スタミナ
	float _stamina;
	// スタミナの最大値
	float _staminaMax;
	// スタミナを消費中かどうか
	bool _isConsumingStamina;
	// スタミナが尽きたかどうか
	bool _isTired;
	// スタミナの1フレームあたりの回復速度
	float _staminaRecoverySpeed;

	// 移動可能かどうか
	bool _canMove;
	// 移動速度
	float _moveSpeed;
	// 移動速度（フレームデータを使った移動）
	float _moveSpeedFWD;

	// 攻撃状態かどうか
	bool _isAttackState;
	// 鉄球の攻撃コリジョンが有効かどうか
	bool _enabledIBAttackCollision;

	// 鉄球
	Chain* _chain;
	// 鉄球が追従状態かどうか
	IB_MOVE_STATE _ibMoveState;

	Capsule _capsuleCollision;

	/* アニメーション関連 */
	// モーション変更可能かどうか
	bool  _canMotionCancel;
	// 次のコンボモーションを再生するかどうか
	bool _playNextComboAnim;

	// アニメーションマネージャ
	AnimationManager* _animManager;
	// アニメーション情報のマップコンテナ
	static std::map<int, ANIMATION_INFO> _animMap;
	// 戦闘待機状態の残りフレーム数
	int _idleFightingRemainingCnt;

	ANIM_STATE _animStatus;

	// フレームデータ
	FrameData* _frameData;


	VECTOR _blastOffDir;
	float _blastOffPower;


	int _rightHandFrameIndex;


	bool _isSwinging;
	bool _isRotationSwinging;
	int _rotationCnt;


	ModelColor* _modelColor;


	static Player* _instance;

	//------------
	//齋藤が書きました。
	bone* _bone[2];// 0:leftHear 1:RightHear
	int _nowLevel;//現在のレベルが入ります。
	int _nowExp; //現在持っている経験値を格納します。
	int _maxLevel;//レベルの最大値
	std::map<int, int> _nextLevel;// first 現在のレベル  second  次のレベルが上がるまでの経験値

	int _power;//吹っ飛ばす力です。
	std::map<int, std::pair<int, float>> _powerAndScale;//攻撃力と拡大率を格納したコンテナです。
	//------------
};