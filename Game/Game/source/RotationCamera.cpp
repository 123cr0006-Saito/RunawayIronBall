//----------------------------------------------------------------------
// @filename RotationCamera.cpp
// @date: 2024/03/13
// @author: saito ko
// @explanation
// カメラを回転させる処理をするクラス
//----------------------------------------------------------------------
#include "RotationCamera.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param distance カメラと注視点の距離
// @param targetPos 注視点の位置
// @return 無し
//----------------------------------------------------------------------
RotationCamera::RotationCamera(float distance,VECTOR targetPos) : CameraBase() {
	_pointDistance.z = -distance;
	_targetPos = targetPos;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
RotationCamera::~RotationCamera() {};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているか
//----------------------------------------------------------------------
bool RotationCamera::Process() {

	float _cameraDirYAdd = 0.01f;
	_cameraDirY += _cameraDirYAdd;

	//カメラの位置を計算
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	//行列の掛け算
	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//注視点からの距離に行列を変換する
	VECTOR Vecter = VTransform(_pointDistance, origin);

	//注視点の位置に移動
	VECTOR VecAdd = VAdd(Vecter, _targetPos);

	//カメラのセット
	SetCameraPositionAndTarget_UpVecY(VecAdd, _targetPos);
	return true;
};