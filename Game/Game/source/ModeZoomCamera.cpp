//----------------------------------------------------------------------
// @filename ModeZoomCamera.cpp
// ＠date: 2024/03/09
// ＠author: saito ko
// @explanation
// ゲート表示時にカメラを移動させるクラス
//----------------------------------------------------------------------
#include "ModeZoomCamera.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos カメラの位置
//----------------------------------------------------------------------
ModeZoomCamera::ModeZoomCamera(VECTOR pos) {
	_pos = pos;
	_camera = nullptr;
	_time = 0;
	_currentTime = 0;
};

bool ModeZoomCamera::Initialize(){
	int time = 5 * 1000;
	_time = time + 2000; // 移動時間に2秒追加
	_currentTime = GetNowCount();
	//ターゲットの位置
	VECTOR targetPos = _pos; 
	// カメラの次の方向
	 VECTOR camPos= GetCameraPosition();//カメラの位置取得
	 VECTOR nextDirVec = VSub(_pos, camPos);// ゲートの位置（原点）に向く方向ベクトル作成
	 nextDirVec = VNorm(nextDirVec);//正規化
	 //カメラの今の方向
	 VECTOR nowDirVec = VSub(camPos, Player::GetInstance()->GetPosition());
	 nowDirVec = VNorm(nowDirVec);//正規化
	 // カメラの次の位置
	 VECTOR nextCameraPos = VAdd(VScale(nextDirVec, -1000), VAdd(_pos,VGet(0, 1000, 0)));//ゲートの位置から少し位置を離して高さを足す

	_camera = NEW TargetZoomCamera(targetPos,nextCameraPos,nowDirVec,nextDirVec,time);
	return true;
};

bool ModeZoomCamera::Terminate() {
	delete _camera; _camera = nullptr;
	return true;
}

bool ModeZoomCamera::Process() {
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();

	int nowTime = GetNowCount() - _currentTime;
	_camera->Process();

	if (nowTime >= _time) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};