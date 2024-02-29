#pragma once
#include "appframe.h"
class ModeScenario : public ModeBase
{
	typedef ModeBase base;
public:
	ModeScenario(std::string scenarioFile);
	bool LoadOnceHandleData();
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool SearchLetter(std::string text, int byte);

protected:
	static bool IsLoadHandle;
	static std::unordered_map<int,int>_charaHandleMap;
	static std::unordered_map<int, std::string>_nameHandleMap;
	static std::unordered_map<int, int>_BackGroundHandleMap;
	struct ScenarioData {
		int voiceData;
		int charaHandle;
		int nameHandle;
		int backGroundHandle;
		std::string text;
	};
	XInput* _input;
	std::vector<ScenarioData> _scenarioData;
	int _nowTextByte;
	int _nowTextLine;
	int _currentTime;

};

