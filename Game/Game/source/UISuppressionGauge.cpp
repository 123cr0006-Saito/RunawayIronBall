//----------------------------------------------------------------------
// @filename UISuppressionGauge.cpp
// @date: 2024/02/26
// @author: saito ko
// @explanation
// 制圧ゲージを表示するクラス
//----------------------------------------------------------------------
#include "UISuppressionGauge.h"

const unsigned short UISuppressionGauge::vertex[6]{ 0,1,2,2,1,3 };
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos 位置
// @param size 画像の最大枚数
// @param handle ハンドル
// @return 無し
//----------------------------------------------------------------------
UISuppressionGauge::UISuppressionGauge(VECTOR pos, int size, int* handle) : UIBase::UIBase( pos,  size,  handle){
	_suppressionValue = Suppression::GetInstance();
	//     x,      y,    u,    v
	float posTbl[][4] = {
		{-_cx,-_cy,0.0f,0.0f},
		{_cx,-_cy,1.0f,0.0f},
		{-_cx,_cy,0.0f,1.0f},
		{_cx,_cy,1.0f,1.0f}
	};

	VECTOR center = VAdd(pos, VGet(_cx, _cy, 0));
	// 制圧ゲージの中心座標を設定
	_standardPos = VAdd(center, VGet(posTbl[0][0], posTbl[0][1], 0));
	// 制圧ゲージの初期化
	for (int i = 0; i < 4; i++) {
		//制圧部分のゲージ
		_suppression[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_suppression[i].u = posTbl[i][2];
		_suppression[i].v = posTbl[i][3];
		_suppression[i].dif = GetColorU8(255, 255, 255, 255);
		_suppression[i].rhw = 1.0f;
		//残りのゲージ
		_residue[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_residue[i].u = posTbl[i][2];
		_residue[i].v = posTbl[i][3];
		_residue[i].dif = GetColorU8(125, 125, 125, 255);
		_residue[i].rhw = 1.0f;
	}

	GetGraphSizeF(handle[2], &_ironHandleSize.first.x, &_ironHandleSize.first.y);
	_ironHandleSize.second = _cy / 2;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
UISuppressionGauge::~UISuppressionGauge() {
	_suppressionValue = nullptr;
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功しているか
//----------------------------------------------------------------------
bool UISuppressionGauge::Process(){
	int nowSuppression = _suppressionValue->GetNowSuppression();
	int maxSuppression = _suppressionValue->GetMaxSuppression();

	float ratio = static_cast<float>(nowSuppression) / maxSuppression;

	// 真ん中の頂点2つの座標とｕを変えたら実装できる
	for (int i = 0; i < 2; i++) {
		_suppression[0 + 2 * i].pos = VAdd(_standardPos, VScale(VGet(_cx * ratio, _cy * i, 0),2));
		_suppression[0 + 2 * i].u =  ratio;
		_residue[1 + 2 * i].pos = VAdd(_standardPos, VScale(VGet(_cx * ratio, _cy * i, 0), 2));
		_residue[1 + 2 * i].u = ratio;
	}
	_ironHandleSize.first = VAdd(_standardPos, VScale(VGet(_cx * ratio, 0 , 0), 2));
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているか
//----------------------------------------------------------------------
bool UISuppressionGauge::Draw() {
	VERTEX2D* list[2] = { _suppression,_residue};
	// 制圧ゲージの描画
	for (int i = 0; i < 2; i++) {
		DrawPrimitiveIndexed2D(list[i], 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[i], true);
	}
	// 鉄球の描画
	DrawRotaGraph(_ironHandleSize.first.x, _ironHandleSize.first.y + _ironHandleSize.second, 0.5f,0.0f,_handle[2], true);
	return true;
};