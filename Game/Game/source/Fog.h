//----------------------------------------------------------------------
// @filename Fog.h
// @date: 2024/02/22
// @author: saito ko
// @explanation
// �v���C���[�����߂�ꂽ�͈͊O�ɏo��ƃt�H�O����������N���X
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include "player.h"

class Fog
{
	public:
	Fog();
	~Fog();
	void Process(int stageNum);
	void UpdateIsFog(int stageNum);
	bool GetIsFog() { return IsFog; }
protected:
	Player* _player;//	�v���C���[�N���X
	bool IsFog;//�t�H�O���������Ă��邩�ǂ���
	const int _fogDistanceMax = 30000;//�t�H�O����������ő勗��
	const int _fogDistanceMin = 2000;//�t�H�O����������ŏ�����
	float _fogEndDistance;//�t�H�O���������鋗��
	int _param;//���݂̃t�H�O�̔Z��
	int _easingCount;//�C�[�W���O�̃J�E���g
};

