//----------------------------------------------------------------------
// @filename AnimationChain.h
// @date: 2024/03/16
// @author: saito ko
// @explanation
// ゲームクリア時のリザルト画面で鎖のアニメーションを表示するクラス
//----------------------------------------------------------------------
#pragma once
#include <math.h>
#include "../../Appframe/source/System/Header/Function/Easing.h"
#include "../../Appframe/source/System/Header/Resource/ResourceServer.h"
#include "dxlib.h"
class AnimationChain
{
public : 
	AnimationChain(VECTOR pos, float angle);
	~AnimationChain();
	void Process();
	void Draw();
protected:
	int _handle;//画像ハンドル
	int _frameCount;//フレームカウント
	int _maxCount;//最大フレーム数
	float _angle;//角度
	float _startX,_startY,_endX,_endY;// 始点と終点
	float _nowX,_nowY;// 現在の座標
};

