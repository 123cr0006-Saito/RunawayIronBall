#include "appframe.h"
#include "ModeGame.h"
class ModeClear : public ModeBase
{
	typedef ModeBase base;
public:
	ModeClear();
	ModeClear(ModeGame* mode);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void AnimProcess();
	void ValuationProcess();
	void Valuation();

protected:
	int _model;
	int _attachAnim;
	int _frameCount;
	int _maxCount;
	std::unordered_map<std::string,int> _handle;
	int _alphaValue;
	int _currentTime;
	XInput* input;
	ModeGame* _modeGame;
	int _valuationHandle[4];
	int _valuationTime;
	int _valuation;
	int _timeHandle[10];
};

