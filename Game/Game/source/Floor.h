//----------------------------------------------------------------------
// @filename Floor.cpp
// ��date: 2024/03/03
// ��author: saito ko
// @explanation
// �Q�[���X�e�[�W���鏰(�^�C��)���Ǘ�����N���X
//----------------------------------------------------------------------
#pragma once
#include "myJson.h"
#include "appframe.h"
class Floor
{
public :
	Floor();
	~Floor();
	void Delete();
	void Create(myJson jsonFile, int stageNum);
	std::vector<std::string> LoadName(int stageNum);
	void Render();
protected:
	std::vector<int> _modelList;//���f���̃��X�g
	std::vector<std::string> _nameList;//���f���̖��O���X�g
};

