#include "House.h"

std::vector<int>House:: _breakFrame;

House::House() :BuildingBase::BuildingBase() {
}

House::~House()
{
}

bool House::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	if (!base::Init(modelHandle, startPos, rotation, scale)){ return false;}

	// 当たり判定の設定
	obb.length[0] = obbLength.x * scale.x;
	obb.length[1] = obbLength.y * scale.y;
	obb.length[2] = obbLength.z * scale.z;

	MATRIX mRot = MGetRotX(rotation.x);
	mRot = MMult(mRot, MGetRotY(rotation.y));
	mRot = MMult(mRot, MGetRotZ(rotation.z));

	obb.dir_vec[0] = VTransform(obb.dir_vec[0], mRot);
	obb.dir_vec[1] = VTransform(obb.dir_vec[1], mRot);
	obb.dir_vec[2] = VTransform(obb.dir_vec[2], mRot);

	obb.pos = VAdd(startPos, VGet(0.0f, obb.length[1] / 2.0f, 0.0f));


	//// _breakFrameが空の場合は初期化
	//if (_breakFrame.empty()) {
	//	for (int i = 0; i < MV1GetFrameNum(_modelHandle); i++) {

	//		// フレームの名前を取得する
	//		std::string frameName = MV1GetFrameName(_modelHandle, i);
	//		std::string checkName = "BRK";
	//		if (frameName.substr(0, 3) != checkName) {
	//			continue;
	//		}

	//		_breakFrame.push_back(i);

	//		// フレームの座標変換行列を取得する
	//		MATRIX mFrameLocal = MV1GetFrameLocalMatrix(_modelHandle, i);
	//		// フレームのローカル座標を求める
	//		VECTOR vFrameLocalPos = VGet(0.0f, 0.0f, 0.0f);
	//		vFrameLocalPos = VTransform(vFrameLocalPos, mFrameLocal);

	//		// モデルの起点座標から見たフレームの方向を取得する
	//		VECTOR vFrameLocalDir = VGet(0.0f, 0.0f, -1.0f);
	//		if (VSquareSize(vFrameLocalPos) > 0.0000f) {
	//			vFrameLocalDir = VNorm(vFrameLocalPos);
	//			// 吹っ飛ぶ方向をz軸マイナス方向に限定する
	//			if (vFrameLocalDir.z > 0.0f) {
	//				vFrameLocalDir.z *= -1;
	//			}
	//		}

	//		//FRAME_INFO f = { i, vFrameLocalDir };
	//		//_frameInfo.push_back(f);

	//		//_locus.resize(_locus.size() + 1);
	//	}
	//}

	return true;
}

bool House::Process()
{
	if(!base::Process()) { return false; }
	return true;
}

bool House::Render()
{
	if (!base::Render()) { return false; }	
	return true;
}

bool House::DrawDebugInfo()
{
	if (!base::DrawDebugInfo()) { return false; }
	if (_useCollision) {
		obb.Render(GetColor(255, 255, 255));
	}
	_breakObj->DrawDebugInfo();
	return true;
}
