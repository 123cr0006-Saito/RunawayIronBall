#pragma once

// アニメーション情報
struct ANIMATION_INFO {
	// アニメーションのインデックス番号
	int animIndex;
	// ループ回数（0なら無限ループ）
	int loopTimes;
};

// アニメーションアイテムクラス
class AnimationItem
{
private:
	friend class AnimationManager;
	AnimationItem();
	~AnimationItem();

	// アニメーションアイテムの初期設定
	void Setup(int animIndex, int attachIndex, float totalTime, int loopTimes);

	int _stateNo;

	// アニメーションのアタッチインデックス
	int _attachIndex;
	// アニメーションの総再生時間
	float _totalTime;
	// アニメーションの再生時間
	float _playTime;

	// モーションブレンド用
	float _closeTotalTime;
	float _closeTime;
	float _openTotalTime;
	float _openTime;
	

	// ループカウント用
	int _loopCnt;
};