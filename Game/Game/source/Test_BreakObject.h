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

	// 吹っ飛ばしの中心方向を指定
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
		// モデルの起点座標から見たフレームの方向
		VECTOR dir;

		// デバッグ用
		VECTOR pos, startPos;
	};

	std::vector<FRAME_INFO> _frameInfo;
	VECTOR _blastDir;
	float _blastPower;


	// デバッグ用
	//struct FRAME_POSITION
	//{
	//	VECTOR pos, startPos;
	//};
	//std::vector<FRAME_POSITION> _framePos;
};