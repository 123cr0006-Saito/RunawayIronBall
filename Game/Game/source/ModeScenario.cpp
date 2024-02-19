#include "ModeScenario.h"
ModeScenario::ModeScenario(std::string scenarioFile) {
	CFile ScenarioFile(scenarioFile);
	if (ScenarioFile.Success()) {
		const char* data = (const char*) ScenarioFile.Data();
		int c = 0;
		int size = ScenarioFile.Size();
		while (c < size) {
			ScenarioData scenario;
			// 表示するキャラの番号を取得する
			c += GetDecNum(&data[c], &scenario.charaHandle);
			// 表示する名前のハンドル番号を取得する
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.nameHandle);
			// textを取得
			c += FindString(&data[c], ',', &data[size]); c++;  c += GetString(&data[c], &scenario.text);
			// 改行などスキップ
			c += SkipSpace(&data[c], &data[size]);
			_scenarioData.push_back(scenario);
		}
	}
#ifdef _DEBUG
	else {
		std::string message = "ファイルが見つかりません";
		MessageBox(NULL, message.c_str(), "エラー", MB_OK);
	}
#endif
};

bool ModeScenario::Initialize(){
	if (!base::Initialize()) { return false; }
	index = 0;
	_currentTime = GetNowCount();
	return true;
};

bool ModeScenario::Terminate(){
	base::Terminate();
	_scenarioData.clear();
	return true;
};

bool ModeScenario::Process(){
	base::Process();
	return true;
};

bool ModeScenario::SearchLetter(std::string text, int byte) {
	if (text.at(byte) <= 0x7f) {
		return true;
	}
	return false;
};

bool ModeScenario::Render() {
	base::Render();
	float speed = 120.0f / 60.0f;
	int nowTime = GetNowCount() - _currentTime;
	if (index < _scenarioData.at(1).text.length() && nowTime >= speed) {
		if (SearchLetter(_scenarioData.at(1).text, index)) {
			index++;
		}
		else {
			index += 2;
		}
		_currentTime = GetNowCount();
	}
	std::string copy = _scenarioData.at(1).text.substr(0, index);

	clsDx();
	DrawFormatString(100,100,GetColor(0,0,0),"%s", copy.c_str());

	return true;
};