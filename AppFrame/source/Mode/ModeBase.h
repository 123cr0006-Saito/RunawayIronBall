#pragma once
#include "../System/Header/Input/XInput.h"
#include "../System/Header/Function/Fps.h"
#include "../System/Header/Effect/Effect.h"
#include <math.h>
#include <string>
class ModeServer;

class		ModeBase
{
public:
	ModeBase();
	virtual ~ModeBase();

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Process();
	virtual bool	Render();


public:
	int	GetModeCount() { return _cntMode; }			// ���̃��[�h���n�܂��Ă���̃J�E���^
	unsigned long GetModeTm() { return _tmMode; }	// ���̃��[�h���n�܂��Ă���̎���ms
	unsigned long GetStepTm() { return _tmStep; }	// �O�t���[������̌o�ߎ���ms

	void SetCallPerFrame(int frame) { _callPerFrame = _callPerFrame_cnt = frame; }	// ���t���[����1��Process()���ĂԂ�(def:1)
	void SetCallOfCount(int count) { _callOfCount = count; }		// 1��̌Ăяo���ɉ���Process()���ĂԂ�(def:1)
	int GetCallPerFrame() { return _callPerFrame; }
	int GetCallOfCount() { return _callOfCount; }

	XInput* GetInputAdd() { return input; }


private:
	friend	ModeServer;
	// ModeServer�p
	std::string		_szName;
	int				_uid;
	int				_layer;

	void StepTime(unsigned long tmNow);
	void StepCount();

	// ���[�h�������p
	int		_cntMode;	// �{���[�h�ɓ����Ă���̃J�E���^, 0�X�^�[�g
	unsigned long	_tmMode;	// �{���[�h�ɓ����Ă���̎��ԁBms
	unsigned long	_tmStep;	// �O�t���[������̌o�ߎ��ԁBms
	unsigned long	_tmModeBase;	// ���̃��[�h���n�܂������ԁBms
	unsigned long	_tmPauseBase;	// �|�[�Y���̃x�[�X���ԁBms
	unsigned long	_tmPauseStep;	// �|�[�Y���̐ώZ���ԁBms ���쒆�A���̒l�� _tmMode �ɗp����
	unsigned long	_tmOldFrame;	// �O�t���[���̎��ԁBms

	// CallPerFrame / CallOfCount�p
	int		_callPerFrame, _callPerFrame_cnt;
	int		_callOfCount;
	//Input
	XInput* input;
	Fps* fps;
	Effect* effect;
};

