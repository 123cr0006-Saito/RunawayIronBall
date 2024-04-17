//----------------------------------------------------------------------
// @filename LoadingChain.h
// ��date: 2024/03/03
// ��author: saito ko
// @explanation
// loading���̍��𐧌�E�`����s���N���X
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include "ModelColor.h"
#include "../AppFrame/source/System/Header/Function/mymath.h"
class LoadingChain
{
public:
	LoadingChain();
	~LoadingChain();
	void Init();
	void Process();
	void AnimProcess();
	void SetPlayerModelHandle(int handle);
	void Render();

	void SetBallPosition(VECTOR pos) { _iPos = pos; }

protected:
	// ���̌�
	static const int Chain_Num = 5;
	//��
	int _cModelHandle;
	VECTOR _cPos[Chain_Num];

	// �S��
	int _iModelHandle;
	VECTOR _iPos;
	VECTOR _ibDefaultScale;
	int _attackAnimCnt;
	int _animIndex;
	float _animTotalTime;
	float _playTime;

	// ���̑�
	int _playerModelHandle;
	float _length;

	// �z�u�\�P�b�g
	int _socketNo[3];

	ModelColor* _modelColor; // 2�̃��f���̐F��ύX���邪���ɖ߂��Ȃ����߃C���X�^���X�͈�ɂ���
};

