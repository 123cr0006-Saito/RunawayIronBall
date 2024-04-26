//----------------------------------------------------------------------
// @filename AfterImage.h
// ��date: 2024/04/26
// ��author: Morozumi Hiroya
// @explanation
// �L�����N�^�[�̎c���\�����s���N���X
// �������ɂ������f���𕡐��̕\�����邱�ƂŎc����\������
//----------------------------------------------------------------------
#pragma once
#include "AppFrame.h"
#include <vector>

class AfterImage
{
public:
	AfterImage();
	~AfterImage();

	// ����������
	// @param modelHandle: ���f���n���h��
	// @param afterImageNum: �����ɕ\������c���̍ő吔
	void Init(int modelHandle, int afterImageNum = 10);

private:	
	struct ModelInfo
	{
		// �g�p�t���O
		bool use;
		// �c��̎�������
		int remainTime;

		// ----------------------------------------
		// ���f�����
		
		// ���f���n���h��
		int modelHandle;
		// �����x
		int alpha;

		// ----------------------------------------
	};

	std::vector<ModelInfo*> _modelInfo;
};