#pragma once
#include "appframe.h"
#include "AnimationItem.h"

#include <map>
#include <vector>


// アニメーション管理クラス
class AnimationManager
{
public:
	AnimationManager(int modelHandle);
	~AnimationManager();

	// アニメーション情報用マップコンテナを追加する
	// 引数: mapコンテナへのポインタ（各クラスの静的メンバ変数）
	void InitMap(std::map<int, ANIMATION_INFO>* animMap);

	// ANIMATION_INFO型のアニメーション情報の初期設定を行う
	void SetupAnimationInfo(int statusNo, int animIndex, int loopTimes);

	// アニメーションアイテムを追加する
	void AddAnimationItem(int statusNo);

	// アニメーションの再生処理
	void Process(int StatusNo);

private:
	// モデルハンドル
	int _modelHandle;

	// 再生中アニメーションのステータス番号
	int _animNo;

	// アニメーション情報用マップコンテナ
	// キー : int型 キャラクターのステータス番号
	// バリュー : ANIMATION_INFO型 アニメーション情報
	std::map<int, ANIMATION_INFO>* _animMap;

	// アニメーションアイテム
	std::vector<AnimationItem*> _animContainer;

	// アニメーションの再生時間
	float _playTime;

};