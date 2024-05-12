//----------------------------------------------------------------------
// @filename TimeLimit.h
// @date: 2024/02/27
// @author: saito ko
// @explanation
// �������Ԃ̊Ǘ����s���N���X
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include <math.h>
class TimeLimit
{
	public:
	TimeLimit();
	~TimeLimit();
	void SetTimeLimit(int minutes, int second);
	
	float Process();
	int SecondsToTime();
	void Stop();
	void Restart();
	
	static TimeLimit* _instance;
	static TimeLimit* GetInstance() { return _instance; }

	float GetRemainingTime() { return _remainingTime; }
	int GetTimeLimit() { return SecondsToTime(); }
	int GetElapsedTime() { return GetElapsedSecond(); }
	int GetElapsedSecond();

	int GetStartTime() { return _setTime / 1000; }

protected:
	float _startTime,_timeLimit;// �J�n���ԂƐ�������
	float _setTime;// �ݒ莞��
	float _remainingTime;// �c�莞��
	int _stopTime;// �X�g�b�v��������
	bool _IsStop;// ���Ԃ��~�߂邩�ǂ���
};

