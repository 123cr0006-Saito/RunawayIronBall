//----------------------------------------------------------------------
// @filename AnimationItem.h
// @date: 2024/02/07
// @author: Morozumi Hiroya
// @explanation
// キャラクターのアニメーション情報を保持するクラス
// AnimationManagerクラスで、このクラスのインスタンスを追加・削除し、アニメーションの管理を行う
//----------------------------------------------------------------------
#pragma once

// アニメーション情報
struct ANIMATION_INFO {
	// アニメーションのインデックス番号（mv1ファイルで設定されている番号）
	int animIndex;
	// ループ回数（0なら無限ループ）
	int loopTimes;
};

class AnimationItem
{
private:
	friend class AnimationManager;
	AnimationItem();
	~AnimationItem();

	// アニメーションアイテムの初期設定
	void Setup(int animIndex, int attachIndex, float totalTime, int loopTimes);

	// 各キャラクターごとに設定されているアニメーションのステータス番号（enum型からint型にキャストしたもの）
	int _stateNo;

	// アニメーションのアタッチインデックス
	int _attachIndex;
	// アニメーションの総再生時間
	float _totalTime;
	// アニメーションの再生時間
	float _playTime;

	// --------------------------------------------------------------------------------------------
	// モーションブレンド関連

	// アニメーション開始時
	float _openTotalTime;
	float _openTime;
	// アニメーション終了時
	float _closeTotalTime;
	float _closeTime;
	// --------------------------------------------------------------------------------------------

	// 残りループ回数
	// 0なら無限ループ
	int _loopCnt;
};