//----------------------------------------------------------------------
// @filename BoardArrow.cpp
// ＠date: 2024/02/01
// ＠author: saito ko
// @explanation
// 指定の方向に矢印を描画するクラス
//----------------------------------------------------------------------
#include "BoardArrow.h"
const unsigned short ::BoardArrow::vertexList[12] = {0,1,2,2,1,3,4,5,6,6,5,7};
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param name: ファイル名
// @param length: 矢印の長さ
// @return なし
//----------------------------------------------------------------------
BoardArrow::BoardArrow(std::string name,float length, int animMax):
	_handle(ResourceServer::LoadGraph("Arrow", name.c_str())),
	_animMax(animMax)
{
	// 初期化
	_animCount = 0;
	// uvリスト
	float uvList[VERTEX_MAX][2] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f}
	};
	
	// 横の長さ
	float Width = 50.0f;
	// 縦の長さ
	_length = length;
	float posListX[VERTEX_MAX] = { -Width, Width, -Width, Width, -Width, Width, -Width, Width};
	float posListZ[VERTEX_MAX] = { 0.0f, 0.0f,_length, _length, _length, _length, _length, _length};
	// 頂点の設定
	for (int i = 0; i < VERTEX_MAX; i++) {
		_originPos[i] = VGet(posListX[i], 50, posListZ[i]);
		vertex[i].norm = VGet(0,1,0);
		vertex[i].dif = GetColorU8(255, 255, 255, 255);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
		vertex[i].u = uvList[i][0];
		vertex[i].v = uvList[i][1];
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
	}
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
BoardArrow::~BoardArrow() {};
//----------------------------------------------------------------------
// @brief 頂点の更新
// @return なし
//----------------------------------------------------------------------
void BoardArrow::UpdateVertex() {
	float Width = 50.0f;
	float ratio = static_cast<float>(_animCount) / _animMax;
	for (int i = 0; i < 2; i++) {
		VECTOR dirVec = _originPos[i+6];
		_originPos[i + 2].z = VScale(dirVec, ratio).z;
		_originPos[i + 4].z = VScale(dirVec, ratio).z;
		vertex[i].v = ratio;
		vertex[i+6].v = ratio;
	}
};
//----------------------------------------------------------------------
// @brief 更新処理
// @param pos: 座標
// @param dirY: y軸の回転角度
// @return 成功したかどうか
//----------------------------------------------------------------------
bool BoardArrow::Process(VECTOR pos, float dirY) {
	// アニメーションを進める
	_animCount++;
	if (_animCount > _animMax) {
		_animCount = 0;
	}
	// 頂点情報を更新
	UpdateVertex();
	// y軸行列を使用し回転させる
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < VERTEX_MAX; i++) {
		vertex[i].pos = VAdd(pos,VTransform(_originPos[i], matrix));
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @param pos: 座標
// @param dirVec: 方向ベクトル
// @return 成功したかどうか
//----------------------------------------------------------------------
bool BoardArrow::Process(VECTOR pos, VECTOR dirVec) {
	// 方向ベクトルから角度を出し、y軸行列で回転させる
	float dirY = atan2(dirVec.x, dirVec.z);
	// アニメーションを進める
	_animCount++;
	if (_animCount > _animMax) {
		_animCount = 0;
	}
	// 頂点情報を更新
	UpdateVertex();
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < VERTEX_MAX; i++) {
		vertex[i].pos = VAdd(pos, VTransform(_originPos[i], matrix));
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool BoardArrow::Render() {
	SetUseBackCulling(false);
	DrawPolygonIndexed3D(vertex, VERTEX_MAX, vertexList, 4, _handle, true);
	SetUseBackCulling(true);
	return true;
};