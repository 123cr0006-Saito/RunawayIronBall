#pragma once
#include "appframe.h"
#include "CharacterBase.h"

class Player : public CharacterBase
{
private:
	enum class STATUS {
		NONE,
		SWING01,
		SWING02,
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



	void UpdateCollision();

	Capsule GetCollision() { return _capsuleCollision; };


	void SetBlastOffPower(VECTOR dir, float power) { _blastOffDir = dir; _blastOffPower = power; };


	VECTOR GetRightHandPos();


	bool GetIsSwing() { return _isSwing; }
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
	

	bool _isSwing;

	static Player* _instance;
};
