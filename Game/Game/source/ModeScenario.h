#pragma once
#include "appframe.h"
class ModeScenario : public ModeBase
{
	typedef ModeBase base;
public:
	ModeScenario(std::string scenarioFile);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool SearchLetter(std::string text, int byte);

protected:
	struct ScenarioData {
		int charaHandle;
		int nameHandle;
		std::string text;
	};
	XInput* _input;
	std::vector<ScenarioData> _scenarioData;
	int _nowTextByte;
	int _nowTextLine;
	int _currentTime;
};

