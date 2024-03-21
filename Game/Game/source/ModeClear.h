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
	void StagingProcess();
	void ValuationProcess();
	void Valuation();

protected:
	XInput* input;
	// モデル
	int _model;
	int _attachAnim;
	int _frameCount;
	int _maxCount;
	// ハンドル
	std::unordered_map<std::string,int> _handle;
	int _valuationHandle[4];
	int _timeHandle[10];
	ModeGame* _modeGame;
	// ステージング
	float _valuationSize;
	int _valuationTime;
	float _nowValuationTime;
	int _valuation;
	bool _IsStaging;
	int _alphaValue;
	int _currentTime;
};

