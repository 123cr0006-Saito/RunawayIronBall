//----------------------------------------------------------------------
// @filename ModeScenario.h
// ＠date: 2024/02/20
// ＠author: saito ko
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
	static bool IsLoadHandle;
	static std::unordered_map<int,int>_charaHandleMap;
	static std::unordered_map<int, std::string>_nameHandleMap;
	static std::unordered_map<int, int>_backGroundHandleMap;
	static std::unordered_map<int, int> _textBoxHandle;
	struct ScenarioData {
		std::string voiceData;
		int charaHandle;
		int nameHandle;
		int backGroundHandle;
		int textBoxHandle;
		std::string text;
	};
	XInput* _input;
	std::vector<ScenarioData> _scenarioData;
	int _scenarioNum;
	int _nameFontHandle;
	int _textFontHandle;
	int _nowTextByte;
	int _nowTextLine;
	int _currentTime;
	int _handleX, _handleY;
	int _skipHandle;
};

