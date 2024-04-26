//----------------------------------------------------------------------
// @filename AfterImage.h
// ＠date: 2024/04/26
// ＠author: Morozumi Hiroya
// @explanation
// キャラクターの残像表現を行うクラス
// 半透明にしたモデルを複数体表示することで残像を表現する
//----------------------------------------------------------------------
#pragma once
#include "AppFrame.h"
#include <vector>

class AfterImage
{
public:
	AfterImage();
	~AfterImage();

	// 初期化処理
	// @param modelHandle: モデルハンドル
	// @param afterImageNum: 同時に表示する残像の最大数
	void Init(int modelHandle, int afterImageNum = 10);

private:	
	struct ModelInfo
	{
		// 使用フラグ
		bool use;
		// 残りの持続時間
		int remainTime;

		// ----------------------------------------
		// モデル情報
		
		// モデルハンドル
		int modelHandle;
		// 透明度
		int alpha;

		// ----------------------------------------
	};

	std::vector<ModelInfo*> _modelInfo;
};