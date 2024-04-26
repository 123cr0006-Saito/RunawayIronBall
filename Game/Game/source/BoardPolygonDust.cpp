//----------------------------------------------------------------------
// @filename BoardPolygonDust.cpp
// @date: 2024/02/28
// @author: saito ko
// @explanation
// 砂煙の板ポリを生成するのクラス
//----------------------------------------------------------------------
#include "BoardPolygonDust.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos 生成位置
// @param sizeX サイズ
// @param handle ハンドル
// @param handleMax ハンドル数
// @param speed 速度
// @param animspeed アニメーションスピード
// @return なし
//----------------------------------------------------------------------
BoardPolygonDust::BoardPolygonDust(VECTOR pos, int sizeX, int* handle, int handleMax, int speed,int animspeed) 
: PlaneEffectBase(pos,  sizeX, handle, handleMax, animspeed) {
	_standardPos = pos;
	_speed = speed;
	VECTOR oPos = VGet(0, 0, 0);
	_vLength = oPos;
	for (int i = 0; i < _dustMax; i++){
		_dustPos[i] = oPos;
	}
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
BoardPolygonDust::~BoardPolygonDust() {
	PlaneEffectBase::~PlaneEffectBase();
};
//----------------------------------------------------------------------
// @brief メイン処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool BoardPolygonDust::Process() {
	PlaneEffectBase::Process();
	// 速度を加算
	_vLength.y += static_cast<float>(_speed) / 2;
	_vLength.z += static_cast<float>(_speed);
	// 行列を生成
	MATRIX matrix = MGetTranslate(_vLength);
	VECTOR originPos = VGet(1, 1, 1);
	// 生成位置を計算
	for (int i = 0; i < _dustMax; i++) {
		float rotation = 360.0f / _dustMax * i * DX_PI / 180.0f;
		matrix = MMult(matrix, MGetRotY(rotation));
		VECTOR vec = VTransform(originPos, matrix);
		_dustPos[i] = VAdd(_standardPos, vec);
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool BoardPolygonDust::Render() {
	for (int i = 0; i < _dustMax; i++) {
		DrawBillboard3D(_dustPos[i],0.5f,0.3f,_sizeX,0.0f, _handle[_animCount],true);
	}
	return true;
};
