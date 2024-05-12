//----------------------------------------------------------------------
// @filename AnimationManager.h
// @date: 2024/02/07
// @author: Morozumi Hiroya
// @explanation
// キャラクターのアニメーションを管理するクラス
// AnimationItemを追加・削除し、アニメーションの管理を行う
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "AnimationItem.h"
#include "MotionList.h"

#include <map>
#include <vector>

// アニメーション情報
// int : 各キャラクターのアニメーションステータス番号（enum型からint型にキャストしたもの）
// ANIMATION_INFO : アニメーション情報（AnimationItem.hで定義）
typedef std::map<int, ANIMATION_INFO> ANIM_MAP;

typedef std::string CHARA_NAME;

class AnimationManager
{
public:	

	AnimationManager();
	~AnimationManager();

	// --------------------------------------------------------------------------------------------
	// アニメーション情報用マップコンテナの初期設定を行う
	// 既に同じキャラクター名が登録されている場合は、同一のマップを使用する

	// @param charaName キャラクター名
	// @param modelHandle モデルハンドル
	// @param fileName モーションリストファイル名
	void InitMap(CHARA_NAME charaName, int modelHandle, std::string fileName);

	// @param charaName キャラクター名
	// @param modelHandle モデルハンドル
	// @param motionList モーション名の配列
	void InitMap(CHARA_NAME charaName, int modelHandle, const std::vector<MotionNamePair>& motionList);
	// --------------------------------------------------------------------------------------------

	// アニメーションの更新処理
	void Process(int statusNo);

	// 最後に追加された（最新の）アニメーションの再生時間を取得する
	float GetPlayTime() { return _playTime; }

	// デバッグ情報の描画
	void DrawDebugInfo();

private:
	// アニメーションアイテムを追加する
	void AddAnimationItem(int statusNo);

	// 全キャラクターのアニメーション情報用マップコンテナ
	// 同種のキャラクターのアニメーション情報を共有するため、キャラクター名をキーにしてマップを保持する
	static std::map<CHARA_NAME, ANIM_MAP> _allCharaAnimMap;

	// アニメーション情報用マップコンテナ
	// _allCharaAnimMapから指定したキャラクターのアニメーション情報を取得するためのマップ
	// キー : int型 キャラクターのステータス番号
	// バリュー : ANIMATION_INFO型 アニメーション情報
	ANIM_MAP* _targetAnimMap;

	// モデルハンドル
	int _modelHandle;

	// 再生中アニメーションのステータス番号
	int _animNo;

	// アニメーションアイテム
	std::vector<AnimationItem*> _animContainer;

	// 最後に追加された（最新の）アニメーションアイテム
	AnimationItem* _latestAnimItem;
	// 最後に追加された（最新の）アニメーションの再生時間
	float _playTime;
};