#pragma once
#include "appframe.h"
#include "AnimationItem.h"

#include <map>
#include <vector>



class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

	// ANIMATION_INFO型のアニメーション情報の初期設定を行う
	void SetupAnimationInfo(int statusNo, int animIndex, int loopTimes);

	// アニメーションアイテムを追加する
	void AddAnimationItem(int statusNo);

	// アニメーションの再生処理
	void Process();

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