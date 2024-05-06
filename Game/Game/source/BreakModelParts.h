//----------------------------------------------------------------------
// @filename BreakModelParts.h
// @date: 2024/01/07
// @author: Morozumi Hiroya
// @explanation
// 建物モデルをパーツごとに吹き飛ばす処理を行うクラス
// あらかじめモデルのパーツを分割しておき、そのパーツ分けを利用して吹き飛ばす処理を行う
// パーツの名前の先頭に「BRK」がついているものを吹き飛ばす対象とし、それ以外のパーツはその場にとどまる 
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"

class BreakModelParts
{
public:
	BreakModelParts();
	~BreakModelParts();

	// 初期化処理
	void Init(int modelHandle);
	// 更新処理
	void Process();

	// 吹き飛ばし処理の有効状態かどうかを取得する
	bool GetIsActive() { return _isActive; }

	// 吹き飛ばし処理の有効化設定
	// @param activate : 吹き飛ばし処理を有効化するかどうか
	// @param _blastDir : 吹き飛ばし方向
	void Activate(bool activate, VECTOR blastDir = VGet(0.0f,0.0f,0.0f));

	// デバッグ情報の表示
	void DrawDebugInfo();

private:

	// 吹き飛ばし方向をセットする
	// パーツごとに、吹き飛ばし方向と1フレームあたりの回転地を設定する
	// @param vDir : 吹き飛ばしの中心方向
	void SetBlastDir(VECTOR vDir);
	// パーツごとに適応した行列をリセットし、モデルを初期状態に戻す
	void ResetFrameMatrix();


private:
	// パーツの吹き飛ばし処理を行うかどうか
	bool _isActive;
	// パーツの吹き飛ばし処理が終了したかどうか
	bool _finishedBreakAll;

	// モデルハンドル
	// 各建物クラスのインスタンスから受け取る（そのインスタンスとモデルハンドルを共有する）
	int _modelHandle;
	// モデルの回転値
	VECTOR _modelRotation;

	struct FRAME_INFO {
		// 3Dモデルのパーツのインデックス番号
		int frameIndex;

		// 吹き飛ばし処理が終了したかどうか
		bool finishedBreak;

		// --------------------------------------------------------------------------------------------
		// 吹き飛ばし情報
		VECTOR horizontalDir; // 水平方向
		float horizontalVelocity; // 水平方向の速度
		float verticalVelocity; // 鉛直方向の速度
		VECTOR vRot; // 各軸における1フレームの回転値
		// --------------------------------------------------------------------------------------------
	};

	std::vector<FRAME_INFO*> _frameInfo;


	// デバッグ表示用
	// 吹き飛ばし方向
	VECTOR _blastDir;
	// 軌跡表示をするかどうか
	bool _isDrawLocus;
	// 軌跡表示用の座標情報
	std::vector<std::vector<VECTOR>> _locus;
};
