//----------------------------------------------------------------------
// @filename ModeScenario.h
// @date: 2024/02/20
// @author: saito ko
// @explanation
// シナリオを再生するクラス
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
	static bool IsLoadHandle;// 画像を読み込んだかどうか
	static std::unordered_map<int,int>_charaHandleMap;// キャラクターハンドルの管理コンテナ
	static std::unordered_map<int, std::string>_nameHandleMap;// キャラクター名の管理コンテナ
	static std::unordered_map<int, int>_backGroundHandleMap;// 背景の管理コンテナ
	static std::unordered_map<int, int> _textBoxHandle;// テキストボックスの管理コンテナ
	// シナリオデータ
	struct ScenarioData {
		std::string voiceData;
		int charaHandle;
		int nameHandle;
		int backGroundHandle;
		int textBoxHandle;
		std::string text;
	};
	XInput* _input;// 入力クラス
	std::vector<ScenarioData> _scenarioData;// シナリオデータの管理コンテナ
	int _scenarioNum;// シナリオの番号
	int _nameFontHandle;// 名前のフォントハンドル
	int _textFontHandle;// テキストのフォントハンドル
	int _nowTextByte;// 現在のテキストのバイト数
	int _nowTextLine;// 現在のテキストの行数
	int _currentTime;// 現在の時間
	int _handleX, _handleY;// 画像の座標
	int _skipHandle;// スキップボタンの画像
};

