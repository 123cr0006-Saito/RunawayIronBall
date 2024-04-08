//----------------------------------------------------------------------
// @filename BoardPolygon.cpp
// ＠date: 2024/02/28
// ＠author: saito ko
// @explanation
// 板ポリを生成するためのクラス
//----------------------------------------------------------------------
#include "BoardPolygon.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos: ポリゴンの中心点
// @param dir: z軸(正面)の方向
// @param sizeX: 横軸の長さ
// @param handle: 画像ハンドル
// @param handleMax: アニメーションの切り替え数
// @param speed: アニメーションスピード
// @return なし
//----------------------------------------------------------------------
BoardPolygon::BoardPolygon(VECTOR pos, VECTOR dir, int sizeX, int* handle, int handleMax, int speed)
	: PlaneEffectBase(pos, sizeX, handle, handleMax, speed) {

	//直交座標系を求める
	VECTOR PolyVecX = VNorm(VCross(dir, VGet(0, -1, 0)));
	VECTOR PolyVecY = VNorm(VCross(dir, PolyVecX));

	VECTOR VecSize[4] = {
		VAdd(VAdd(VScale(PolyVecX,-sizeX / 2),VScale(PolyVecY, _sizeY / 2)),pos),
		VAdd(VAdd(VScale(PolyVecX, sizeX / 2),VScale(PolyVecY, _sizeY / 2)),pos),
		VAdd(VAdd(VScale(PolyVecX,-sizeX / 2),VScale(PolyVecY,-_sizeY / 2)),pos),
		VAdd(VAdd(VScale(PolyVecX, sizeX / 2),VScale(PolyVecY,-_sizeY / 2)),pos)
	};
	//uvリスト
	float uvList[4][2] = {
	{0.0 ,0.0 },
	{1.0f,0.0 },
	{0.0 ,1.0f},
	{1.0f,1.0f}
	};
	//初期化
	for (int i = 0; i < 4; i++) {
		_vertex[i].pos = VecSize[i];
		//向かっている方向の逆なので*-1
		_vertex[i].norm = VScale(dir, -1);
		_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		_vertex[i].u = uvList[i][0];
		_vertex[i].v = uvList[i][1];
		_vertex[i].su = 0.0f;
		_vertex[i].sv = 0.0f;
	}

};
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos: ポリゴンの中心点
// @param matrix: 回転行列
// @param sizeX: 横軸の長さ
// @param handle: 画像ハンドル
// @param handleMax: アニメーションの切り替え数
// @param speed: アニメーションスピード
// @return なし
//----------------------------------------------------------------------
BoardPolygon::BoardPolygon(VECTOR pos, MATRIX matrix, int sizeX, int* handle, int handleMax, int speed)
	:PlaneEffectBase(pos, sizeX, handle, handleMax, speed) {

	// 頂点の座標を求める
	VECTOR VecSize[4] = {
	VAdd(pos,VTransform(VGet(-sizeX / 2, _sizeY / 2,0), matrix)),
	VAdd(pos,VTransform(VGet(sizeX / 2,  _sizeY / 2,0), matrix)),
	VAdd(pos,VTransform(VGet(-sizeX / 2,-_sizeY / 2,0), matrix)),
	VAdd(pos,VTransform(VGet(sizeX / 2, -_sizeY / 2,0), matrix))
	};
	//uvリスト
	float uvList[4][2] = {
		{0.0 ,0.0 },
		{1.0f,0.0 },
		{0.0 ,1.0f},
		{1.0f,1.0f}
	};

	//行列からフォワードベクトルを取り出す
	VECTOR norm = Math::MatrixToVector(matrix, 2);
	//初期化
	for (int i = 0; i < 4; i++) {
		_vertex[i].pos = VecSize[i];
		_vertex[i].norm = norm;
		_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		_vertex[i].u = uvList[i][0];
		_vertex[i].v = uvList[i][1];
		_vertex[i].su = 0.0f;
		_vertex[i].sv = 0.0f;
	}

};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
BoardPolygon::~BoardPolygon() {
	PlaneEffectBase::~PlaneEffectBase();
};
//----------------------------------------------------------------------
// @brief メイン処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool BoardPolygon::Process() {
	PlaneEffectBase::Process();
	return true;
};
//----------------------------------------------------------------------
// @brief 描画
// @return 成功したかどうか
//----------------------------------------------------------------------
bool BoardPolygon::Render() {
	if (_IsPlay) {
		DrawPolygonIndexed3D(_vertex, 4, vertexOrder, 2, _handle[_animCount], true);
	}
	return true;
};
