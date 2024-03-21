#pragma once
#include "appframe.h"
#include "ModeInstructions.h"
#include "ModeTitle.h"
#include <array>

class ModePause : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void SetVertex();

	void SelectSetVolum(int& setVolum);//サウンドの設定
	void SelectSetVibration();//コントローラーの振動設定
	void SelectOperationInstructions();//操作説明画面
	void SelectGameEnd();//ゲームを終了する

	void VertexProcess();

protected:
	XInput* _input;
	int gauss_handle;
	int _selectItem;
	//-----------------------------
	//多分グローバルで作るか、それぞれのクラスを直接オプションでいじるか
	int _bgmVolum;
	int _seVolum;
	bool _isVibration;
	static const int MAX_MODE = 5;

	VERTEX2D _seGauge[4];
	VERTEX2D _bgmGauge[4];

	static const unsigned short vertex[6];

	std::unordered_map<std::string, int> _handleMap;
};