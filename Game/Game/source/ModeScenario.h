//----------------------------------------------------------------------
// @filename ModeScenario.h
// @date: 2024/02/20
// @author: saito ko
// @explanation
// �V�i���I���Đ�����N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class ModeScenario : public ModeBase
{
	typedef ModeBase base;
public:
	ModeScenario(std::string scenarioFile,int scenarioNum);
	bool LoadOnceHandleData();
	void ScenarioUniqueProcess();
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool SearchLetter(std::string text, int byte);

protected:
	static bool IsLoadHandle;// �摜��ǂݍ��񂾂��ǂ���
	static std::unordered_map<int,int>_charaHandleMap;// �L�����N�^�[�n���h���̊Ǘ��R���e�i
	static std::unordered_map<int, std::string>_nameHandleMap;// �L�����N�^�[���̊Ǘ��R���e�i
	static std::unordered_map<int, int>_backGroundHandleMap;// �w�i�̊Ǘ��R���e�i
	static std::unordered_map<int, int> _textBoxHandle;// �e�L�X�g�{�b�N�X�̊Ǘ��R���e�i
	// �V�i���I�f�[�^
	struct ScenarioData {
		std::string voiceData;
		int charaHandle;
		int nameHandle;
		int backGroundHandle;
		int textBoxHandle;
		std::string text;
	};
	XInput* _input;// ���̓N���X
	std::vector<ScenarioData> _scenarioData;// �V�i���I�f�[�^�̊Ǘ��R���e�i
	int _scenarioNum;// �V�i���I�̔ԍ�
	int _nameFontHandle;// ���O�̃t�H���g�n���h��
	int _textFontHandle;// �e�L�X�g�̃t�H���g�n���h��
	int _nowTextByte;// ���݂̃e�L�X�g�̃o�C�g��
	int _nowTextLine;// ���݂̃e�L�X�g�̍s��
	int _currentTime;// ���݂̎���
	int _handleX, _handleY;// �摜�̍��W
	int _skipHandle;// �X�L�b�v�{�^���̉摜
};

