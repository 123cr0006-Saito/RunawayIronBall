//----------------------------------------------------------------------
// @filename EffekseerRotation.h
// @date: 2024/03/07
// @author: saito ko
// @explanation
// ��]�U���̃G�t�F�N�g���Đ�����N���X
//----------------------------------------------------------------------
#pragma once
#include "EffekseerBase.h"
class EffekseerRotation :  public EffekseerBase
{
	typedef EffekseerBase base;
public:
	EffekseerRotation(int handle, VECTOR* pos, float size, VECTOR* rotation, float height = 0.0f, float speed = 1.0f, bool loopFlag = false);
	~EffekseerRotation()override;
	bool Process()override;
	bool Render()override;
	void SetLoop(bool Loop) { _loopFlag = Loop;}
protected: 
	static const int _maxEffect = 2;//�G�t�F�N�g�̕\���ő吔
	int _playingEffectHandle[_maxEffect];//�G�t�F�N�g�̃n���h��
	int _currentTime;//�G�t�F�N�g�̍Đ�����
	VECTOR* _pos;// �G�t�F�N�g�̍��W
	VECTOR* _rotation;// �G�t�F�N�g�̉�]�l
};

