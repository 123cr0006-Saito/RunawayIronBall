//----------------------------------------------------------------------
// @filename Afterglow.cpp
// @date: 2024/12/14
// @author: saito ko
// @explanation
// それぞれのカメラクラスで共通で使用する変数がある基底クラス
//----------------------------------------------------------------------
#include "CameraBase.h"
const float CameraBase::_cameraMinDistance = -150.0f;
const float CameraBase::_cameraMaxDistance = -2500.0f;
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return なし
//----------------------------------------------------------------------
CameraBase::CameraBase(){
	_input = XInput::GetInstance();//コンストラクタで入力用のクラスのポインタを取得
	_pointDistance = VGet(0, 0, -600);
	_currentTime = 0;
	_cameraDirX = 0.53f;
	_cameraDirY = 0.0f;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
CameraBase::~CameraBase() {
	_input = nullptr;
};
//----------------------------------------------------------------------
// @brief カメラの処理
// @return なし
//----------------------------------------------------------------------
bool CameraBase::Process() {
	return true;
};