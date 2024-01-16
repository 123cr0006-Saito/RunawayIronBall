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

	// 吹っ飛ばしの中心方向をセットする
	void SetBlastDir(VECTOR vDir);


	void DrawDebugInfo();

private:
	int _modelHandle;
	VECTOR _pos;

	int _breakCnt;

	struct FRAME_INFO {
		// 3Dモデルのパーツのインデックス番号
		int frameIndex;

		// 吹き飛ばし情報

		VECTOR horizontalDir; // 水平方向
		float horizontalVelocity; // 水平方向の速度
		float verticalVelocity; // 鉛直方向の速度

		VECTOR rotVector; // 各軸における1フレームの回転値
	};

	std::vector<FRAME_INFO> _frameInfo;
	VECTOR _blastDir;
	float _blastPower;


	// 軌跡表示をするかどうか
	bool _isDrawLocus;
	// 軌跡表示用の座標情報
	std::vector<std::vector<VECTOR>> _locus;
};
