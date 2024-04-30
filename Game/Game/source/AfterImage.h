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
	// @param remainTime: 残像の持続時間
	void Init(int parentModelHandle, std::string keyName, std::string modelName, int afterImageNum = 10, int remainTime = 10);

	// 残像を追加する
	// @param animIndex: アニメーションのインデックス（mv1ファイル内でのインデックス番号）（-1の場合はアニメーションを適応しない）
	// @param playTime: アニメーションの再生時間
	void AddAfterImage(int animIndex = -1, float playTime = 0);

	// 更新処理
	void Process();
	
	// 描画処理
	void Render();

private:	
	// 残像モデルの情報
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

		// ディフューズカラーのスケール値
		// マテリアルのディフューズカラーに掛け合わせる値
		// RGBA(0.0f ~ 1.0f)
		COLOR_F difColorScale;

		// アニメーションのアタッチインデックス番号
		int attachIndex;
		// ----------------------------------------
	};
	std::vector<ModelInfo*> _modelInfo;

	// 親モデルのハンドル
	int _parentModelHandle;

	// 同時に表示する残像の最大数
	int _afterImageNum;
	// 残像の持続時間
	int _remainTimeMax;
};