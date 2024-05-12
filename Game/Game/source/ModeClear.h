//----------------------------------------------------------------------
// @filename ModeClear.h
// @date: 2024/03/16
// @author: saito ko
// @explanation
// �N���A���Ƀ��U���g�Ǝ��̃X�e�[�W�ւ̑J�ڂ��Ǘ�����N���X
//----------------------------------------------------------------------
#include "appframe.h"
#include "AnimationChain.h"
#include "ModeGame.h"
class ModeClear : public ModeBase
{
	typedef ModeBase base;
public:
	ModeClear(int elapsedTime,int startTime);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void AnimProcess();
	void ValuationProcess();
	void Valuation(int elapsedTime,int startTime);

protected:
	XInput* _input;
	// ���f��
	int _model;
	int _attachAnim;
	int _frameCount;
	int _maxCount;
	// �n���h��
	std::unordered_map<std::string,int> _handle;
	int _valuationHandle[4];
	int _timeHandle[10];
	// �X�e�[�W���O
	float _valuationSize;
	int _valuationTime;
	float _nowValuationTime;
	int _valuation;
	bool _IsStaging;
	bool _IsNextStage;
	float _alphaValue;
	int _currentTime;

	std::vector<AnimationChain*> _chain;
};

