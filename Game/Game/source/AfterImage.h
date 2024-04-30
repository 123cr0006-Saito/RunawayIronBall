//----------------------------------------------------------------------
// @filename AfterImage.h
// @date: 2024/04/26
// @author: Morozumi Hiroya
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
	// @param parentModelHandle: �e���f���̃n���h��
	// @param keyName: �L�[���iResourceServer�ł̃��f���f�[�^�̊Ǘ��Ɏg�p����j
	// @param modelName: ���f���̃p�X
	// @param afterImageNum: �����ɕ\������c���̍ő吔
	// @param remainTime: �c���̎�������
	void Init(int parentModelHandle, std::string keyName, std::string modelName, int afterImageNum = 10, int remainTime = 10);

	// �c����ǉ�����
	void AddAfterImage();

	// �X�V����
	void Process();
	
	// �`�揈��
	void Render();

private:	
	// �c�����f���̏��
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
		// �f�B�t���[�Y�J���[�̃X�P�[���l
		// �}�e���A���̃f�B�t���[�Y�J���[�Ɋ|�����킹��l
		// RGBA(0.0f ~ 1.0f)
		COLOR_F difColorScale;
		// ----------------------------------------
	};
	std::vector<ModelInfo*> _modelInfo;

	// �e���f���̃n���h��
	int _parentModelHandle;

	// �����ɕ\������c���̍ő吔
	int _afterImageNum;
	// �c���̎�������
	int _remainTimeMax;
};