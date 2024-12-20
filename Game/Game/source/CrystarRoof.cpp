//----------------------------------------------------------------------
// @filename CrystarRoof.cpp
// @date: 2024/02/24
// @author: saito ko
// @explanation
// クライスターの屋根部分を描画するためのクラス
//----------------------------------------------------------------------
#include "CrystarRoof.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param model モデルハンドル
// @param parent 親モデルハンドル
// @param frameName 屋根を配置する位置を検索するためのフレーム名
// @return 無し
//----------------------------------------------------------------------
CrystarRoof::CrystarRoof(int model, int parent,std::string frameName) {
	_model = model;
    _parent = parent;
	_frame = MV1SearchFrame(parent,"Hip");
	_joint = frameName;
	Update();
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
CrystarRoof:: ~CrystarRoof() {};
//----------------------------------------------------------------------
// @brief 更新
// @return 無し
//----------------------------------------------------------------------
bool CrystarRoof::Update(){
	// 行列の初期化
	MV1SetMatrix(_model, MGetIdent());

	// 行列の更新
	// 座標の取得
	int frame = MV1SearchFrame(_model, _joint.c_str());
	VECTOR framePos = MV1GetFramePosition(_model, frame);
	// 取得した座標を原点にするための平行移動行列を作成
	MATRIX transMatrix = MGetTranslate(VScale(framePos,-1.0f));
	// 親フレームのアタッチする場所の行列を取得
    MATRIX frameMatrix = MV1GetFrameLocalWorldMatrix(_parent,_frame);

	//行列を合成
	MATRIX MixMatrix = MMult(transMatrix,frameMatrix);

	// 合成した行列をアタッチするモデルにセット
	MV1SetMatrix(_model, MixMatrix);
	return true;
};
//----------------------------------------------------------------------
// @brief 描画
// @return 無し
//----------------------------------------------------------------------
bool CrystarRoof::Render() {
	MV1DrawModel(_model);
	return true;
};