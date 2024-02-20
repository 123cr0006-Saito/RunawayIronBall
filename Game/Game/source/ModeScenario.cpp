#include "ModeScenario.h"
ModeScenario::ModeScenario(std::string scenarioFile) {
	CFile ScenarioFile(scenarioFile);
	if (ScenarioFile.Success()) {
		const char* data = (const char*) ScenarioFile.Data();
		int c = 0;
		int size = ScenarioFile.Size();
		while (c < size) {
			ScenarioData scenario;
			// text���擾
			c += GetString(&data[c], &scenario.text);
			// �\������L�����̔ԍ����擾����
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.charaHandle);
			// �\�����閼�O�̃n���h���ԍ����擾����
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.nameHandle);
			// ���s�ȂǃX�L�b�v
			c += 2;
			_scenarioData.push_back(scenario);
		}
	}
#ifdef _DEBUG
	else {
		std::string message = "�t�@�C����������܂���";
		MessageBox(NULL, message.c_str(), "�G���[", MB_OK);
	}
#endif
};

bool ModeScenario::Initialize(){
	if (!base::Initialize()) { return false; }
	_nowTextByte = 0;
	_nowTextLine = 0;
	_currentTime = GetNowCount();
	_input = XInput::GetInstance();

	ChangeFont("���C���I");

	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	SetFontSize(32);
	return true;
};

bool ModeScenario::Terminate(){
	base::Terminate();
	_scenarioData.clear();
	_input = nullptr;

	// �t�H���g�֘A�̏�����
	ChangeFontType(DX_FONTTYPE_NORMAL);
	ChangeFont("MS�S�V�b�N");
	SetFontSize(16);
	return true;
};

bool ModeScenario::Process(){
	base::Process();

	// �~���b�P�ʂł̕����̕`��
	float speed = 3.0f / 60.0f * 1000;// �~���b�P��
	int nowTime = GetNowCount() - _currentTime;
	if (_nowTextByte < _scenarioData.at(_nowTextLine).text.length() && nowTime >= speed) {
		if (SearchLetter(_scenarioData.at(_nowTextLine).text, _nowTextByte)) {
			_nowTextByte++;
		}
		else {
			_nowTextByte += 2;
		}
		_currentTime = GetNowCount();
	}

	// ���̃��C���ɍs��
	if (_nowTextByte >= _scenarioData.at(_nowTextLine).text.length()) {
		if (_input->GetTrg(XINPUT_BUTTON_A)) {
			_nowTextLine++;
			_nowTextByte = 0;
			_currentTime = GetNowCount();
		}
	}

	// �V�i���I�����ׂĕ`�悵�I����
	if (_nowTextLine >= _scenarioData.size()) {
		ModeServer::GetInstance()->Del(this);
	}
	
	return true;
};

bool ModeScenario::SearchLetter(std::string text, int byte) {
	if (0x00 <= text.at(byte) && text.at(byte) <= 0x7f) {
		return true;
	}
	return false;
};

bool ModeScenario::Render() {
	base::Render();

	std::string copy = _scenarioData.at(_nowTextLine).text.substr(0, _nowTextByte);

	DrawFormatString(100,100,GetColor(255,255,255),"%s", copy.c_str());

	return true;
};