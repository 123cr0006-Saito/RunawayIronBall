//----------------------------------------------------------------------
// @filename ModeGameOver.h
// @date: 2024/02/15
// @author: saito ko
// @explanation
// ゲームオーバー時に遷移し、リトライかタイトルに戻るかを選択するモード
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
	void AnimProcess();
	void SelectProcess();
protected:
	XInput* _input;// 入力クラス
	std::unordered_map<std::string,int> _handle;// 画像ハンドルの管理コンテナ
	int _selectItem;// 選択中の項目

	bool _selectEnd;// 選択終了
	int _model;// モデルハンドル
	int _modelFrame;// モデルのフレーム
	VECTOR _targetPos;// カメラのターゲット座標
	VECTOR _cameraPos;// カメラの位置
};

