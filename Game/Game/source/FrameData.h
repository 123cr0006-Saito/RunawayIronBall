#pragma once
#include "appframe.h"
#include <map>
#include <string>
#include <vector>

typedef std::pair<int, float> CommandParam;

class FrameData
{
public:
	//�R�}���h
	//�v���C���[�@0�ԑ�
    //�G�l�~�[�@   100�ԑ�
    //�G�t�F�N�g�@1000�ԑ�

	FrameData();
	~FrameData();

	bool LoadData(std::string, std::vector<std::pair<int, std::string>> frameData);

	void Process(int state, int animTime);

	std::vector<CommandParam> GetCommandData() { return _nextCommandList; };

	static std::map<std::string,std::map<int, std::multimap<int, CommandParam>>> _kindFrameData;//�L�����N�^�[�̎�ޕʂɎ��f�[�^

protected:
	//map key _state : multimap key _animTime : _commandParam
	std::map<int, std::multimap<int, CommandParam>>_frameData;//���ꂼ�ꂪ���t���[���f�[�^
	std::vector<CommandParam> _nextCommandList;//���̃t���[���Ŏ��s����R�}���h���X�g
};

