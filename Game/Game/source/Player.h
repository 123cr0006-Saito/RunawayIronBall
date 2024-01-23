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

	void SetBone();//�V����������֐��ł��B bone�̃t���[����T�����߂Ɏg�p����֐��ł��B���json�ł̓ǂݍ��݂ɂ��邩������܂���B
	//���V����������֐��ł��B�ǂ���json�ǂݍ��݂��ǂ��ɏ����΂悢�̂��킩��Ȃ������̂ŁA�R���X�g���N�^�̎��ɌĂяo���֐��Ƃ��Ď������܂����B
	void SetNextExp(std::string FileName);//�o���l�f�[�^�̓ǂݍ���
	bool  UpdateExp();//�o���l���z���Ă������A���x�����グ��B
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
	//�V���������܂����B
	bone* _bone[2];// 0:leftHear 1:RightHear
	int _nowLevel;//���݂̃��x��������܂��B
	int _nowExp; //���ݎ����Ă���o���l���i�[���܂��B
	int _maxLevel;//���x���̍ő�l
	std::map<int, int> _nextLevel;// first ���݂̃��x��  second  ���̃��x�����オ��܂ł̌o���l

};
