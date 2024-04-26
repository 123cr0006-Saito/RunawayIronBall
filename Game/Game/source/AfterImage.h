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
	void Init(int parentModelHandle, std::string keyName, std::string modelName, int afterImageNum = 10);


	void SetAfterImage(MATRIX parentMatrix);

	
	void Render();

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
		// �����x�i0.0 ~ 1.0�j
		float alpha;

		// ----------------------------------------
	};

	std::vector<ModelInfo*> _modelInfo;
	int _afterImageNum;

	int _parentModelHandle;
};