//----------------------------------------------------------------------
// @filename AfterImage.h
// @date: 2024/04/26
// @author: Morozumi Hiroya
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
	// @param parentModelHandle: 親モデルのハンドル
	// @param keyName: キー名（ResourceServerでのモデルデータの管理に使用する）
	// @param modelName: モデルのパス
	// @param afterImageNum: 同時に表示する残像の最大数
	void Init(int parentModelHandle, std::string keyName, std::string modelName, int afterImageNum = 10);


	void SetAfterImage(MATRIX parentMatrix);

	
	void Render();

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
		// 透明度（0.0 ~ 1.0）
		float alpha;

		// ----------------------------------------
	};

	std::vector<ModelInfo*> _modelInfo;
	int _afterImageNum;

	int _parentModelHandle;
};