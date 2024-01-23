#pragma once
#include "appframe.h"
#include "CharacterBase.h"
#include "bone.h"
#include "myJson.h"

class Player : public CharacterBase
{
private:
	enum class STATUS {
		NONE,
		WAIT,
		HORISONTAL_SWING,
		RUN,
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
	                                                             
	void UpdateBone();
	void UpdateCollision();

	Capsule GetCollision() { return _capsuleCollision; };


	void SetBlastOffPower(VECTOR dir, float power) { _blastOffDir = dir; _blastOffPower = power; };


	VECTOR GetRightHandPos();


	bool GetIsSwing() { return _isSwinging; }
	static Player* GetInstance() { return _instance; }

	void DrawDebugInfo();
private:
	XInput* _input;


	float _speed;

	Capsule _capsuleCollision;



	STATUS _animStatus;
	int _attach_index;
	float _total_time;
	float _play_time;


	VECTOR _blastOffDir;
	float _blastOffPower;


	int _rightHandFrameIndex;
	

	bool _isSwinging;

	static Player* _instance;

	//------------
	//齋藤が書きました。
	bone* _bone[2];// 0:leftHear 1:RightHear
	int _nowLevel;//現在のレベルが入ります。
	int _nowExp; //現在持っている経験値を格納します。
	int _maxLevel;//レベルの最大値
	std::map<int, int> _nextLevel;// first 現在のレベル  second  次のレベルが上がるまでの経験値

};
