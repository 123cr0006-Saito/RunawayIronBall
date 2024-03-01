#include "ModeScenario.h"

bool ModeScenario::IsLoadHandle = false;
std::unordered_map<int, int> ModeScenario::_charaHandleMap;
std::unordered_map<int, std::string> ModeScenario::_nameHandleMap;
std::unordered_map<int, int> ModeScenario::_backGroundHandleMap;
std::unordered_map<int, int> ModeScenario::_textBoxHandle;

ModeScenario::ModeScenario(std::string scenarioFile) {

	LoadOnceHandleData();

	CFile ScenarioFile(scenarioFile);
	if (ScenarioFile.Success()) {
		const char* data = (const char*) ScenarioFile.Data();
		int c = 0;
		int size = ScenarioFile.Size();
		while (c < size) {
			ScenarioData scenario;
			// textを取得
			c += GetString(&data[c], &scenario.text);
			// voiceを取得する
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.voiceData);
			// 表示するキャラの番号を取得する
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.charaHandle);
			// 表示するキャラの番号を取得する
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.nameHandle);
			// 背景の番号を取得する
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.backGroundHandle);
			// 改行などスキップ
			c += 2;
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

bool ModeScenario::LoadOnceHandleData() {
	//読み込みが終わっている状態
	if (IsLoadHandle) {return true;}

	// 初回だけ画像ハンドルを読み込む
	std::string commonPath = "res/Scenario/";

	// キャラ画像ハンドル
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
				// ハンドルの名前を取得
				c += GetString(&data[c], &handleName);
				// keyを取得する
				c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &key);
				// 改行コードや空白を埋める
				c += SkipSpace(&data[c], &data[size]);
				std::string handlePath = commonPath + Path + "/" + handleName + ".png";
				_charaHandleMap[key] = ResourceServer::LoadGraph(handleName, handlePath);
			}
		}
	}

	// 名前の読み込み
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
				// 名前を取得
				c += GetString(&data[c], &charaName);
				// keyを取得する
				c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &key);
				// 改行コードや空白を埋める
				c += 2;
				_nameHandleMap[key] = charaName;
			}
		}
	}

	// 背景の画像の読み込み
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
				// ハンドルの名前を取得
				c += GetString(&data[c], &handleName);
				// keyを取得する
				c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &key);
				// 改行コードや空白を埋める
				c += SkipSpace(&data[c], &data[size]);
				std::string handlePath = commonPath + Path + "/" + handleName + ".png";
				_backGroundHandleMap[key] = ResourceServer::LoadGraph(handleName, handlePath);
			}
		}
	}

	// テキストボックスの読み込み
	{
		std::string Path = "TextBox";
		std::string scenarioFile = commonPath + "Data/" + Path + ".csv";
		CFile ScenarioFile(scenarioFile);
		if (ScenarioFile.Success()) {
			const char* data = (const char*)ScenarioFile.Data();
			int c = 0;
			int size = ScenarioFile.Size();
			while (c < size) {
				std::string handleName;
				int key;
				// ハンドルの名前を取得
				c += GetString(&data[c], &handleName);
				// keyを取得する
				c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &key);
				// 改行コードや空白を埋める
				c += SkipSpace(&data[c], &data[size]);
				std::string handlePath = commonPath + Path + "/" + handleName + ".png";
				_textBoxHandle[key] = ResourceServer::LoadGraph(handleName, handlePath);
			}
		}
	}

	IsLoadHandle = true;

	return true;
};

bool ModeScenario::Initialize(){
	if (!base::Initialize()) { return false; }
	_nowTextByte = 0;
	_nowTextLine = 0;
	_currentTime = GetNowCount();
	_input = XInput::GetInstance();

	GetGraphSize(_charaHandleMap[_scenarioData.at(_nowTextLine).charaHandle], &_handleX, &_handleY);
	_textFontHandle = CreateFontToHandle("メイリオ", 32, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	_nameFontHandle = CreateFontToHandle("メイリオ", 64, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	return true;
};

bool ModeScenario::Terminate(){
	base::Terminate();
	_scenarioData.clear();
	_input = nullptr;

	// フォント関連の初期化
	ChangeFontType(DX_FONTTYPE_NORMAL);
	ChangeFont("MSゴシック");
	return true;
};

bool ModeScenario::Process(){
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	// ミリ秒単位での文字の描画
	float speed = 3.0f / 60.0f * 1000;// ミリ秒単位
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

	// 次のラインに行く
	if (_nowTextByte >= _scenarioData.at(_nowTextLine).text.length()) {
		if (_input->GetTrg(XINPUT_BUTTON_A)) {
			_nowTextLine++;
			_nowTextByte = 0;
			_currentTime = GetNowCount();
		}
	}

	// シナリオをすべて描画し終えた
	if (_nowTextLine >= _scenarioData.size() || _input->GetTrg(XINPUT_BUTTON_START)) {
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
	int x, y;

	// 背景の描画
	DrawGraph(0, 0, _backGroundHandleMap[_scenarioData.at(_nowTextLine).backGroundHandle], true);

	// キャラクターの描画
	DrawGraph(0, 1080 - _handleY, _charaHandleMap[_scenarioData.at(_nowTextLine).charaHandle], true);

	//テキストボックスの描画
	GetGraphSize(_textBoxHandle[0], &x, &y);
	DrawGraph(_handleX, 1080 - _handleY/2,  _textBoxHandle[0],true);

	// 名前の描画
	DrawStringToHandle(_handleX + x / 6, 1080 - y - 100,  _nameHandleMap[_scenarioData.at(_nowTextLine).nameHandle].c_str(), GetColor(255, 255, 255), _nameFontHandle);

	// 文字列の描画
	std::string copy = _scenarioData.at(_nowTextLine).text.substr(0, _nowTextByte);
	DrawStringToHandle(_handleX + x / 5, 1080 - y,  copy.c_str(), GetColor(255, 255, 255),_textFontHandle);

	return true;
};