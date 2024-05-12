//----------------------------------------------------------------------
// @filename Afterglow.h
// @date: 2024/04/01
// @author: saito ko
// @explanation
// 鉄球が攻撃時、鉄球の目に残光を発生させるクラス
//----------------------------------------------------------------------
#pragma once
#include <vector>
#include "dxlib.h"
class Afterglow
{
public : 
	Afterglow(int model,int frameNum,float length,int handle,int animCountMax);
	~Afterglow();
	void Process();
	void Render();
	void SetUpdate(bool update) { IsUpdate = update; _oldPos = MV1GetFramePosition(_model, _frameNum);}// 頂点を更新するかどうかを設定し今の位置を取得
protected:
	void SetVertex();// 頂点を設定する関数

	int _handle; // 画像ハンドル
	int _model; // フレームの位置を取得するためのモデル
	int _frameNum; // 取得したい位置のフレーム番号
	int _animCountMax; // アニメーションの総フレーム数
	int _animCount; // アニメーションのカウント
	float _length; // 長さ
	VECTOR _oldPos; // 前回の位置
	bool IsUpdate; // 頂点を更新するかどうか
	std::vector<unsigned short> _vertexIndex;// 頂点のインデックスコンテナ
	std::vector<VERTEX3D> _vertex; // 頂点のコンテナ
	int _subColor;

	bool IsStop;
};

