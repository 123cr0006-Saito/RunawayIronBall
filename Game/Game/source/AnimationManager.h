#pragma once
#include "appframe.h"
#include "AnimationItem.h"

#include <map>
#include <vector>

typedef std::map<int, ANIMATION_INFO> ANIM_MAP;
typedef std::string CHARA_NAME;

// アニメーション管理クラス
class AnimationManager
{
public:
	

	AnimationManager();
	~AnimationManager();

	// アニメーション情報用マップコンテナを追加する
	// 引数: mapコンテナへのポインタ（各クラスの静的メンバ変数）
	void InitMap(std::map<int, ANIMATION_INFO>* animMap);
	void InitMap(CHARA_NAME charaName, int modelHandle, std::string fileName);

	// ANIMATION_INFO型のアニメーション情報の初期設定を行う
	void SetupAnimationInfo(int statusNo, int animIndex, int loopTimes);

	// アニメーションアイテムを追加する
	void AddAnimationItem(int statusNo);

	// アニメーションの再生処理
	void Process(int StatusNo);

	// 最後に追加されたアニメーションの再生時間を取得する
	float GetPlayTime() { return _playTime; }

private:
	static std::map<CHARA_NAME, ANIM_MAP> _animMap;

	// モデルハンドル
	int _modelHandle;

	// 再生中アニメーションのステータス番号
	int _animNo;

	// アニメーション情報用マップコンテナ
	// キー : int型 キャラクターのステータス番号
	// バリュー : ANIMATION_INFO型 アニメーション情報
	ANIM_MAP* _charaAnimMapPtr;

	// アニメーションアイテム
	std::vector<AnimationItem*> _animContainer;


	// 最後に追加されたのアニメーションアイテム
	AnimationItem* _latestAnimItem;
	// 最後に追加されたアニメーションの再生時間
	float _playTime;

};