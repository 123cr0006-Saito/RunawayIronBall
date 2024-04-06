//----------------------------------------------------------------------
// @filename PlaneEffectBase.h
// ��date: 2024/01/30
// ��author: saito ko
// @explanation
// �|���S����\������G�t�F�N�g�̊��N���X
//----------------------------------------------------------------------
#pragma once
#include "EffectBase.h"

	class PlaneEffectBase : public EffectBase
	{
	public:
		PlaneEffectBase(VECTOR pos, int sizeX, int* handle,int handleMax,int speed);
		virtual ~PlaneEffectBase();
		virtual bool Process()override;
		virtual bool Render()override;
		
	protected:
		static const unsigned short vertexOrder[6];// �|���S����`�悷��ۂ̒��_�̏���
		float _sizeX, _sizeY;//�|���S���̉����Əc��
		int* _handle;// �e�N�X�`���n���h��
		int _speed;// �A�j���[�V�����̍Đ����x
		int _currentTime;// ���݂̎���
		int _animCount;// �A�j���[�V�����̃J�E���g
		int _animMax;// �A�j���[�V�����̍ő�l
	};

