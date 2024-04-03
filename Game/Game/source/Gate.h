//----------------------------------------------------------------------
// @filename Gate.h
// ��date: 2024/03/15
// ��author: saito ko
// @explanation
// �v���C���[���G���΃N���A�ɂȂ�I�u�W�F�N�g�N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class Gate
{
public:
	Gate(VECTOR pos, int r, int* handle,int size, int animSpeed,int handlesize );
	~Gate();
	void Process();
	void Draw();
	VECTOR  GetPos() { return _pos; };
	float GetR() { return _r; };
protected:
	VECTOR _pos;
	int* _handle;
	int _r;
	int _size;
	int _handleSize;
    int _animCount;
	int _animSpeed;
	int _currentTime;
};

