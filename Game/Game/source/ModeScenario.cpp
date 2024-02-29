#include "ModeScenario.h"

bool ModeScenario::IsLoadHandle = false;
std::unordered_map<int, int> ModeScenario::_charaHandleMap;
std::unordered_map<int, std::string> ModeScenario::_nameHandleMap;
std::unordered_map<int, int> ModeScenario::_BackGroundHandleMap;

ModeScenario::ModeScenario(std::string scenarioFile) {

	LoadOnceHandleData();

	CFile ScenarioFile(scenarioFile);
	if (ScenarioFile.Success()) {
		const char* data = (const char*) ScenarioFile.Data();
		int c = 0;
		int size = ScenarioFile.Size();
		while (c < size) {
			ScenarioData scenario;
			// text���擾
			c += GetString(&data[c], &scenario.text);
			// voice���擾����
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.voiceData);
			// �\������L�����̔ԍ����擾����
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.charaHandle);
			// �\������L�����̔ԍ����擾����
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.nameHandle);
			// �w�i�̔ԍ����擾����
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.backGroundHandle);
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

bool ModeScenario::LoadOnceHandleData() {
	//�ǂݍ��݂��I����Ă�����
	if (IsLoadHandle) {return true;}

	// ���񂾂��摜�n���h����ǂݍ���
	std::string commonPath = "res/Scenario/";

	// �L�����摜�n���h��
	{
		std::string Path = "CharaHandle";
		std::string scenarioFile = commonPath + "Data/" + Path + ".csv";
		CFile ScenarioFile(scenarioFile);
		if (ScenarioFile.Success()) {
			const char* data = (const char*)ScenarioFile.Data();
			int c = 0;
			int size = ScenarioFile.Size();
			while (c < size) {
				std::string handleName;
				int key;
				// text���擾
				c += GetString(&data[c], &handleName);
				// voice���擾����
				c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &key);
				// ���s�R�[�h��󔒂𖄂߂�
				c += SkipSpace(&data[c], &data[size]);
				std::string handlePath = commonPath + Path + "/" + handleName + ".png";
				_charaHandleMap[key] = ResourceServer::LoadGraph(handleName, handlePath);
			}
		}
	}

	// ���O�̓ǂݍ���
	{
		std::string Path = "NameHandle";
		std::string scenarioFile = commonPath + "Data/" + Path + ".csv";
		CFile ScenarioFile(scenarioFile);
		if (ScenarioFile.Success()) {
			const char* data = (const char*)ScenarioFile.Data();
			int c = 0;
			int size = ScenarioFile.Size();
			while (c < size) {
				std::string charaName;
				int key;
				// text���擾
				c += GetString(&data[c], &charaName);
				// voice���擾����
				c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &key);
				// ���s�R�[�h��󔒂𖄂߂�
				c += SkipSpace(&data[c], &data[size]);
				_nameHandleMap[key] = charaName;
			}
		}
	}

	// �w�i�̉摜�̓ǂݍ���
	{
		std::string Path = "BackGround";
		std::string scenarioFile = commonPath + "Data/" + Path + ".csv";
		CFile ScenarioFile(scenarioFile);
		if (ScenarioFile.Success()) {
			const char* data = (const char*)ScenarioFile.Data();
			int c = 0;
			int size = ScenarioFile.Size();
			while (c < size) {
				std::string handleName;
				int key;
				// text���擾
				c += GetString(&data[c], &handleName);
				// voice���擾����
				c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &key);
				// ���s�R�[�h��󔒂𖄂߂�
				c += SkipSpace(&data[c], &data[size]);
				std::string handlePath = commonPath + Path + "/" + handleName + ".png";
				_charaHandleMap[key] = ResourceServer::LoadGraph(handleName, handlePath);
			}
		}
	}

	return true;
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