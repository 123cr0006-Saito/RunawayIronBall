#pragma once
#include "EffekseerBase.h"
//���W���ω����� effekseer�N���X
class EffekseerPosSynchro : public EffekseerBase
{
	typedef EffekseerBase base;
public :
	EffekseerPosSynchro(std::string name, VECTOR* pos, float size, float speed = 1.0f, bool loopFlag = false);
	~EffekseerPosSynchro();

	virtual bool Process()override;
private :
	const VECTOR* const _pos; //�|�C���^�̐�̒l�̏����������֎~ �ϐ��̍đ�����s��
};

