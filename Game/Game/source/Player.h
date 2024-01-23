#pragma once
#include "appframe.h"
#include "CharacterBase.h"
#include "bone.h"

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

};
