#pragma once
#include "appframe.h"

class BreakObject
{
public:
	BreakObject();
	~BreakObject();

	void Init();

	void Process();
	void Render();

	// 吹っ飛ばしの中心方向をセットする
	void SetBlastDir(VECTOR vDir);


	void DrawDebugInfo();

private:
	int _modelHandle;
	VECTOR _pos;

	// 吹っ飛ばすパーツのindex番号
	int _breakFrameIndex;

	int _breakCnt;
	
	struct FRAME_INFO {

		int frameIndex;

		// 吹き飛ばし

		VECTOR horizontalDir; // 水平方向
		float horizontalVelocity;
		float verticalVelocity;

		VECTOR rotVector;

		// デバッグ用
		VECTOR pos, startPos;
	};

	std::vector<FRAME_INFO> _frameInfo;
	VECTOR _blastDir;
	float _blastPower;


	bool _isDrawLocus;
	// 軌跡表示用の座標情報
	std::vector<std::vector<VECTOR>> _locus;
};