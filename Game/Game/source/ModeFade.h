//----------------------------------------------------------------------
// @filename ModeFade.h
// @date: 2024/02/29
// @author: saito ko
// @explanation
// ステージ遷移時にフェードイン、フェードアウトを行うためのクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"

class ModeFade : public ModeBase
{
protected: 
	typedef ModeBase base;
public:
	ModeFade(int Time,bool FadeIn = false);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process()override;
	virtual bool Render()override;
protected:
	int _alphaFade;// フェードのアルファ値
	int _currentTime;// 現在の時間
	int _fadeTime;// フェードする時間
    bool _isFadeIn;// フェードインかフェードアウトか
};

