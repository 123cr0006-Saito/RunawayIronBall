//----------------------------------------------------------------------
// @filename AnimationChain.h
// @date: 2024/03/16
// @author: saito ko
// @explanation
// �Q�[���N���A���̃��U���g��ʂō��̃A�j���[�V������\������N���X
//----------------------------------------------------------------------
#pragma once
#include <math.h>
#include "../../Appframe/source/System/Header/Function/Easing.h"
#include "../../Appframe/source/System/Header/Resource/ResourceServer.h"
#include "dxlib.h"
class AnimationChain
{
public : 
	AnimationChain(VECTOR pos, float angle);
	~AnimationChain();
	void Process();
	void Draw();
protected:
	int _handle;//�摜�n���h��
	int _frameCount;//�t���[���J�E���g
	int _maxCount;//�ő�t���[����
	float _angle;//�p�x
	float _startX,_startY,_endX,_endY;// �n�_�ƏI�_
	float _nowX,_nowY;// ���݂̍��W
};

