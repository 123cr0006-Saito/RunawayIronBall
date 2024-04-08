//----------------------------------------------------------------------
// @filename EffekseerBase.h
// ��date: 2024/02/8
// ��author: saito ko
// @explanation
// �G�t�F�N�V�A���Đ����邽�߂̊�{�I�ȃN���X
//----------------------------------------------------------------------
#pragma once
#include "EffectBase.h"
#include "EffekseerForDXLib.h"

//effekseer���g�����߂̌��ƂȂ�N���X
class EffekseerBase : public EffectBase
{
public:
	EffekseerBase(std::string name, VECTOR* pos, float size, float height = 0.0f, float speed =1.0f , bool loopFlag = false,bool loadFlag = true);
	EffekseerBase(int handle, VECTOR* pos, float size, float height = 0.0f, float speed = 1.0f, bool loopFlag = false, bool loadFlag = true);
	virtual ~EffekseerBase()override;
	virtual bool Process()override;
	virtual bool Render()override;

protected:
	int _effectResourceHandle;//�G�t�F�N�g�̃��\�[�X�n���h��
	int _playingEffectHandle;//�Đ����̃G�t�F�N�g�n���h��
	VECTOR  _pos;//�G�t�F�N�g�̍��W
	float _height;//�G�t�F�N�g�̍���
	float _speed;//�G�t�F�N�g�̍Đ����x
	float _size;//�G�t�F�N�g�̑傫��
	bool _loopFlag;//�G�t�F�N�g�̃��[�v�t���O
};

typedef EffekseerBase EffekseerFixed; //���W�Œ�̃G�t�F�N�g