//----------------------------------------------------------------------
// @filename AnimationChain.cpp
// @date: 2024/03/16
// @author: saito ko
// @explanation
// ゲームクリア時のリザルト画面で鎖のアニメーションを表示するクラス
//----------------------------------------------------------------------
#include "AnimationChain.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos: 鎖の終点の座標
// @param angle: 鎖の角度
// @return なし
//----------------------------------------------------------------------
AnimationChain::AnimationChain(VECTOR pos,float angle){
	// 初期化
	// 画像の読み込み
	_handle = ResourceServer::LoadGraph("UI_Chain","res/UI/Chain/Kusari_Straight.png");
	_frameCount = 0;
	_maxCount = 60;
	_endX = pos.x;
	_endY = pos.y;
	_angle = angle * DX_PI / 180;
	float distance = 10000;
	_startX = pos.x - cos(_angle) * distance;
	_startY = pos.y - sin(_angle) * distance;
	_nowX = _startX;
	_nowY = _startY;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
AnimationChain::~AnimationChain(){
};
//----------------------------------------------------------------------
// @brief メイン処理
// @return なし
//----------------------------------------------------------------------
void AnimationChain::Process(){
	// フレームカウントが最大値に達するまで、座標を更新
	if (_frameCount < _maxCount) {
		_nowX = _startX + (_endX - _startX) * _frameCount / _maxCount;
		_nowY = _startY + (_endY - _startY) * _frameCount / _maxCount;
		_frameCount++;
	}
};
//----------------------------------------------------------------------
// @brief 終了処理
// @return なし
//----------------------------------------------------------------------
void AnimationChain::Draw(){
	// 画像の描画 鎖の画像が大きかったため 0.8倍に修正
	DrawRotaGraph(_nowX, _nowY, 0.8f, _angle, _handle, true);
};