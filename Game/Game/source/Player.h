#pragma once
#include "appframe.h"
#include "CharacterBase.h"
#include "bone.h"
#include "myJson.h"

#include "AnimationManager.h"
#include "AnimationItem.h"

#include <map>

class Player : public CharacterBase
{
private:
	enum class STATUS {
		NONE,
		WAIT,		
		RUN,

		HORISONTAL_SWING_01,
		HORISONTAL_SWING_02,
		SPIN_SWING,

		_EOT_
	};

public:
	Player(int modelHandle, VECTOR pos);
	~Player() override;

	bool Process(float camAngleY);
	bool AnimProcess(STATUS oldStatus);
	bool BlastOffProcess();
	bool Render() override;

	void SetBone();//齋藤が作った関数です。 boneのフレームを探すために使用する関数です。後でjsonでの読み込みにするかもしれません。
	//↓齋藤が作った関数です。どこにjson読み込みをどこに書けばよいのかわからなかったので、コンストラクタの次に呼び出す関数として実装しました。
	void SetNextExp(std::string FileName);//経験値データの読み込み
	bool  UpdateExp();//経験値が越えていた時、レベルを上げる。
	int GetNowLevel() { return _nowLevel; };
	void SetExp(int getExp) { _nowExp += getExp; };
	//経験値UIで使用しています。
	int GetNowExp() { return _nowExp; }
	int GetNextExp() { return _nextLevel[_nowLevel]; }

	void UpdateBone();
	void UpdateCollision();

	Capsule GetCollision() { return _capsuleCollision; };


	void SetBlastOffPower(VECTOR dir, float power) { _blastOffDir = dir; _blastOffPower = power; };

	bool UpdateNextComboAnim();


	VECTOR GetRightHandPos();

	bool GetIBFollowingMode() { return _ibFollowingMode; }

	bool GetIsAttackState() { return _isAttackState; }
	static Player* GetInstance() { return _instance; }

	void DrawDebugInfo();
private:
	XInput* _input;

	// 移動可能かどうか
	bool _canMove;
	// 移動速度
	float _speed;

	Capsule _capsuleCollision;

	// 鉄球が追従状態かどうか
	bool _ibFollowingMode;

	// 攻撃状態かどうか
	bool _isAttackState;

	// 次のコンボモーションを再生するかどうか
	bool _playNextComboAnim;
	// 次のコンボモーションのステータスを保持する
	STATUS _nextComboAnim;

	int _comboInputAcceptanceFrame;

	// アニメーションマネージャ
	AnimationManager* _animManager;
	// アニメーション情報のマップコンテナ
	static std::map<int, ANIMATION_INFO> _animMap;

	STATUS _animStatus;
	int _attach_index;
	float _total_time;
	float _play_time;


	VECTOR _blastOffDir;
	float _blastOffPower;


	int _rightHandFrameIndex;


	bool _isSwinging;
	bool _isSpinning;
	int _spinCnt;

	static Player* _instance;

	//------------
	//齋藤が書きました。
	bone* _bone[2];// 0:leftHear 1:RightHear
	int _nowLevel;//現在のレベルが入ります。
	int _nowExp; //現在持っている経験値を格納します。
	int _maxLevel;//レベルの最大値
	std::map<int, int> _nextLevel;// first 現在のレベル  second  次のレベルが上がるまでの経験値
	//------------
};