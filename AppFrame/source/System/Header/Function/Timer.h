#pragma once
#include "dxlib.h"
class Timer
{
public:
	Timer();
	bool Start();//���̃N���X���Ăяo�����Ƃ��ɐ錾����
	double TimeElapsed();//�O�̃t���[������̌o�ߎ��Ԃ��v�Z

	//--��
	LONGLONG GetLastTime() { return _lastTime; }
	double GetElapsedTime() { return _elapsedTime; }
private:
	LONGLONG _lastTime;//�O�̃t���[���̍Ō�̎���
	double _elapsedTime;//�O�̃t���[������̌o�ߎ���
};

