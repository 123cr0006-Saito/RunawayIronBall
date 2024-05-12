//----------------------------------------------------------------------
// @filename ModeScenario.cpp
// @date: 2024/02/20
// @author: saito ko
// @explanation
// シナリオを再生するクラス
//----------------------------------------------------------------------
#include "ModeScenario.h"
#include "ModeFadeComeBack.h"
#include "ModeMovie.h"
#include "ModeGame.h"
#include "ModeBossBattle.h"

bool ModeScenario::IsLoadHandle = false;
std::unordered_map<int, int> ModeScenario::_charaHandleMap;
std::unordered_map<int, std::string> ModeScenario::_nameHandleMap;
std::unordered_map<int, int> ModeScenario::_backGroundHandleMap;
std::unordered_map<int, int> ModeScenario::_textBoxHandle;
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param scenarioFile シナリオのデータが入ったファイルのパス
// @param scenarioNum シナリオの番号
//----------------------------------------------------------------------
ModeScenario::ModeScenario(std::string scenarioFile,int scenarioNum) {
	_scenarioNum = scenarioNum;
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
			c += FindString(&data[c], ',', &data[size]); c++; c += GetString(&data[c], &scenario.voiceData);
			// 表示するキャラの番号を取得する
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.charaHandle);
			// 表示するキャラの番号を取得する
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.nameHandle);
			// 背景の番号を取得する
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.backGroundHandle);
			// 背景の番号を取得する
			c += FindString(&data[c], ',', &data[size]); c++; c += GetDecNum(&data[c], &scenario.textBoxHandle);
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
//----------------------------------------------------------------------
// @brief ハンドルの読み込み
// @return 成功しているかどうか
//----------------------------------------------------------------------
bool ModeScenario::LoadOnceHandleData() {
	//読み込みが終わっている状態
	if (IsLoadHandle) {return true;}

	// 初回だけ画像ハンドルを読み込む
	std::string commonPath = "res/ModeScenario/";

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
//----------------------------------------------------------------------
// @brief 初期化処理
// @return 成功しているかどうか
//----------------------------------------------------------------------
bool ModeScenario::Initialize(){
	if (!base::Initialize()) { return false; }
	_nowTextByte = 0;
	_nowTextLine = 0;
	_currentTime = GetNowCount();
	_input = XInput::GetInstance();

	global._soundServer->DirectPlay("B_Scenario");

	GetGraphSize(_charaHandleMap[_scenarioData.at(_nowTextLine).charaHandle], &_handleX, &_handleY);
	_textFontHandle = CreateFontToHandle("メイリオ", 32, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	_nameFontHandle = CreateFontToHandle("メイリオ", 64, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	_skipHandle = ResourceServer::LoadGraph("Skip", "res/ModeScenario/Skip/UI_Skip.png");
	return true;
};
//----------------------------------------------------------------------
// @brief 削除処理
// @return 成功しているかどうか
//----------------------------------------------------------------------
bool ModeScenario::Terminate(){
	base::Terminate();
	_scenarioData.clear();
	_input = nullptr;
	// フォント関連の初期化
	DeleteFontToHandle(_textFontHandle);
	DeleteFontToHandle(_nameFontHandle);
	ChangeFontType(DX_FONTTYPE_NORMAL);
	ChangeFont("MSゴシック");
	return true;
};
//----------------------------------------------------------------------
// @brief シナリオの番号で固有のmodeを作成する
// @return 無し
//----------------------------------------------------------------------
void  ModeScenario::ScenarioUniqueProcess(){
	switch(_scenarioNum){
	case 1 :
		global._soundServer->DirectPlay("Stage01");
		ModeServer::GetInstance()->Add(NEW ModeGame(), 1, "Game");
		break;
	case 2:
		ModeServer::GetInstance()->Add(NEW ModeBossBattle(), 1, "BossBattle");
		break;
	case 3:
		ModeServer::GetInstance()->Add(NEW ModeMovie(), 10, "Movie");
		break;
	}
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているかどうか
//----------------------------------------------------------------------
bool ModeScenario::Process(){
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();

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

	if (_nowTextLine >= (_scenarioData.size() - 1) && _input->GetTrg(XINPUT_BUTTON_A) || _input->GetTrg(XINPUT_BUTTON_START)) {
		ScenarioUniqueProcess();
		if (!ModeServer::GetInstance()->Search("ScenarioFade")) {
			ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(1500, this), 1000, "ScenarioFade");
		}
	}

	// 次のラインに行く
	if (_nowTextByte >= _scenarioData.at(_nowTextLine).text.length() && _nowTextLine < (_scenarioData.size() - 1) ) {
		if (_input->GetTrg(XINPUT_BUTTON_A)) {
			_nowTextLine++;
			_nowTextByte = 0;
			_currentTime = GetNowCount();
			std::string voiceName = _scenarioData.at(_nowTextLine).voiceData;
			if (voiceName != "") {
				global._soundServer->DirectPlay(voiceName);
			}
		}
	}
	
	return true;
};
//----------------------------------------------------------------------
// @brief 文字が半角か全角かを判定する
// @param text 文字列
// @param byte 文字の位置
// @return 半角か全角か
//----------------------------------------------------------------------
bool ModeScenario::SearchLetter(std::string text, int byte) {
	if (0x00 <= text.at(byte) && text.at(byte) <= 0x7f) {
		return true;
	}
	return false;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているか
//----------------------------------------------------------------------
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
	// スキップボタンの描画
	GetGraphSize(_skipHandle, &x, &y);
	DrawGraph(1920 - x, 1080 - y, _skipHandle, true);
	return true;
};