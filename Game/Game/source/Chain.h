#pragma once
#include "appframe.h"
#include "Player.h"

#define CHAIN_MAX 12

class Chain {
public:

	void Init();
	void Process();

	void MoveProcess();
	void FollowingProcess();
	void PuttingOnSocketProcess();
	void InterpolationProcess();

	void AnimProcess();

	void Render();

	VECTOR GetBallPosition() { return _iPos; }
	float GetBallRadius() { return _r; }

	void DrawDebugInfo();

	//�V�����쐬�����֐��ł�------------------------
	void SetPowerScale(std::string FileName);//�t�@�C���ǂݍ��݂Ń��x���ɍ��킹���U���͂Ɗg�嗦���擾
	bool UpdateLevel();//�v���C���[����擾�����A���x���ōU���͂Ɗg�嗦��ݒ�
	int GetPower() { return _power; }//�m�b�N�o�b�N�p�̗͂�Ԃ��܂��B
	//----------------------------------------------------------


private:
	XInput* _input;

	// ��
	int _cModelHandle;
	VECTOR _cPos[CHAIN_MAX];

	// �S��
	int _iModelHandle;
	VECTOR _iPos;
	VECTOR _iForwardDir;
	float _r = 55.0f; //// ���Sphere�N���X�����


	// �z�u�\�P�b�g
	int _socketNo[3];


	int _attackAnimCnt;


	int _animIndex;
	float _animTotalTime;
	float _playTime;


	float _cnt;
	int _attackDir;

	MATRIX _m[CHAIN_MAX];

	float _length;

	bool _followingMode;
	
	IB_MOVE_STATE _moveState;


	Player* _playerInstance;
	int _playerModelHandle;

	//-------------------
	// �V�����쐬�����ϐ��ł��B
	const int _originR = 50;
	int _power;//������΂��͂ł��B
	std::map<int, std::pair<int, float>> _powerAndScale;//�U���͂Ɗg�嗦���i�[�����R���e�i�ł��B
};