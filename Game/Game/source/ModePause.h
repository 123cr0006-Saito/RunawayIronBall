//----------------------------------------------------------------------
// @filename ModePause.h
// ＠date: 2023/12/25
// ＠author: saito ko
// @explanation
// 音量設定や振動設定、操作説明画面、ゲーム終了を選択するポーズ画面のクラス
//----------------------------------------------------------------------
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
	XInput* _input;// 入力クラス
	int _gaussHandle;// ガウスの画像
	int _selectItem;// 選択中の項目
	int _bgmVolum;// BGMの音量
	int _seVolum;// SEの音量
	static const int MAX_MODE = 5;// 項目の最大数
	VERTEX2D _seGauge[4];// SEのゲージの頂点データ
	VERTEX2D _bgmGauge[4];// BGMのゲージの頂点データ
	static const unsigned short vertex[6];// ポリゴンを描画する際の頂点の順番
	std::unordered_map<std::string, int> _handleMap;// 画像ハンドルの管理コンテナ
};