//----------------------------------------------------------------------
// @filename EffekseerPosSynchro.h
// @date: 2024/02/8
// @author: saito ko
// @explanation
// �ʒu���̂���A�h���X���󂯎��ړ�����G�t�F�N�V�A���Đ�����N���X
//----------------------------------------------------------------------
#pragma once
#include "EffekseerBase.h"
//���W���ω����� effekseer�N���X
class EffekseerPosSynchro : public EffekseerBase
{
	typedef EffekseerBase base;
public :
	EffekseerPosSynchro(std::string name, VECTOR* pos, float size, VECTOR rotation = VGet(0, 0, 0), float height = 0.0f, float speed = 1.0f, bool loopFlag = false);
	EffekseerPosSynchro(int handle, VECTOR* pos, float size, VECTOR rotation = VGet(0,0,0), float height = 0.0f, float speed = 1.0f, bool loopFlag = false);
	~EffekseerPosSynchro()override;
	bool Process()override;
	bool Render()override;
private :
	VECTOR* _pos;//���W�̃A�h���X
	int _currentTime;//�Đ�����
};

