//----------------------------------------------------------------------
// @filename Heart.cpp
// @date: 2024/03/03
// @author: saito ko
// @explanation
// プレイヤーが触れたらＨＰを回復するオブジェクトクラス
//----------------------------------------------------------------------
#include "Heart.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos 位置
// @return 無し
//----------------------------------------------------------------------
Heart::Heart(VECTOR pos){
	_model = ResourceServer::MV1LoadModel("Heart","res/Item/Cg_Object_Heart/Cg_Object_Heart.mv1");

	_sphere.r = 50;
	//posは足元基準で考えています
	_pos = VAdd(pos, VGet(0, _sphere.r * 2, 0));
	_sphere.centerPos = _pos;
	_currentTime = GetNowCount();
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @param 無し
// @return 無し
//----------------------------------------------------------------------
Heart::~Heart() {};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているかどうか
//----------------------------------------------------------------------
bool Heart::Process() {
	float nowTime = GetNowCount() - _currentTime;
	float oneLapTime = 4 * 1000;
	float direction = 2.0f * DX_PI_F * (nowTime / oneLapTime);
	float Sin = sin(direction);

	_sphere.centerPos = VAdd(_pos,VGet(0,_sphere.r * Sin,0));
	MV1SetPosition(_model, _sphere.centerPos);
	MV1SetRotationXYZ(_model,VGet(0, direction,0));
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているかどうか
//----------------------------------------------------------------------
bool Heart::Render() {
	MV1DrawModel(_model);
	return true;
};
