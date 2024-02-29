#pragma once
#include "appframe.h"

class BreakObject
{
public:
	BreakObject();
	~BreakObject();

	void Init(int modelHandle);

	void Process();
	void Render();


	bool GetIsActive() { return _isActive; }
	void Activate(bool activate, VECTOR _blastDir = VGet(0.0f,0.0f,0.0f));




	void DrawDebugInfo();



private:

	// 吹っ飛ばしの中心方向をセットする
	void SetBlastDir(VECTOR vDir);
	void ResetFrameMatrix();


private:
	// パーツの吹っ飛び処理を行うかどうか
	bool _isActive;
	// パーツの吹っ飛び処理が終了したかどうか
	bool _finishedBreakAll;

	// モデルハンドル
	// 各建物クラスのインスタンスから受け取る（そのインスタンスとモデルハンドルを共有する）
	int _modelHandle;
	// モデルの回転値
	VECTOR _modelRotation;

	int _breakCnt;

	struct FRAME_INFO {
		// 3Dモデルのパーツのインデックス番号
		int frameIndex;

		bool finishedBreak;

		// 吹き飛ばし情報
		VECTOR horizontalDir; // 水平方向
		float horizontalVelocity; // 水平方向の速度
		float verticalVelocity; // 鉛直方向の速度
		VECTOR vRot; // 各軸における1フレームの回転値
	};

	std::vector<FRAME_INFO*> _frameInfo;


	/*  */
	// 吹っ飛び方向
	VECTOR _blastDir;


	// 軌跡表示をするかどうか
	bool _isDrawLocus;
	// 軌跡表示用の座標情報
	std::vector<std::vector<VECTOR>> _locus;
};
