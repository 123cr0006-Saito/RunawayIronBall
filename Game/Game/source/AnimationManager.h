#pragma once
#include "appframe.h"
#include "AnimationItem.h"

#include <map>
#include <vector>

// アニメーション情報
struct ANIMATION_INFO {
	// アニメーションのインデックス番号
	int _animIndex;
	// ループ回数（0なら無限ループ）
	int _loopTimes;
};


class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

	// ANIMATION_INFO型のアニメーション情報を追加する
	void SetupAnimationInfo(int statusNo, int animIndex, int loopTimes);

private:
	// モデルハンドル
	int _modelHandle;

	// 再生中のアニメーション番号
	int _animNo;

	// アニメーション情報用マップコンテナ
	// キー : int型 キャラクターのステータス番号
	// バリュー : ANIMATION_INFO型 アニメーション情報
	std::map<int, ANIMATION_INFO> _animInfo;

	// アニメーションアイテム
	std::vector<AnimationItem*> _anim;

	// アニメーションの再生時間
	float _playTime;
};