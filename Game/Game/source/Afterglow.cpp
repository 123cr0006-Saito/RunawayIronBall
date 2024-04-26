//----------------------------------------------------------------------
// @filename Afterglow.cpp
// @date: 2024/04/01
// @author: saito ko
// @explanation
// 鉄球が攻撃時、鉄球の目に残光を発生させるクラス
//----------------------------------------------------------------------
#include "Afterglow.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param model: モデルハンドル
// @param frameNum: モデルハンドルにある位置を取得したいジョイント番号
// @param length: ポリゴンを表示するときのy軸の長さ
// @param handle: 画像ハンドル
// @param animCountMax: アニメーションの流したい時間
// @return なし
//----------------------------------------------------------------------
Afterglow::Afterglow(int model,int frameNum,float length,int handle,int animCountMax){
	// 初期化
	IsUpdate = false;
	_model = model;
	_frameNum = frameNum;
	_length = length;
	_handle = handle;
	_animCount = 0;
	_animCountMax = animCountMax;
	IsStop = false;
	_oldPos = VGet(0,0,0);

	_subColor = static_cast<int>(255 / animCountMax);
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
Afterglow::~Afterglow(){
	_vertex.clear();
	_vertexIndex.clear();
};
//----------------------------------------------------------------------
// @brief 頂点を追加し位置とuv値を設定する
// @return なし
//----------------------------------------------------------------------
void Afterglow::SetVertex(){
	// コンテナの最初に値を追加する関数
	auto vertex_push_front = [&](VERTEX3D index){
		_vertex.insert(_vertex.begin(), index);
	};
	auto index_push_front = [&](unsigned short index){
		_vertexIndex.insert(_vertexIndex.begin(),index);
	};

	// フレームの位置を取得
	VECTOR pos = MV1GetFramePosition(_model,_frameNum);
	// 前の位置との差分を取得
	VECTOR dirVec = VSub(pos, _oldPos);
	if(VSquareSize(dirVec) > 0.01f){
		// 位置が変わっているとき正規化
		dirVec = VNorm(dirVec);
	    // 法線ベクトルを取得
	    VECTOR normalVecX = VNorm(VCross(dirVec,VGet(0,1,0)));
	    // 法線ベクトルを元に上下のベクトルを取得
	    VECTOR normalVecY = VNorm(VCross(dirVec, normalVecX));
	    VECTOR upVec = VScale(normalVecY,_length);
	    VECTOR downVec = VScale(normalVecY, _length * -1);
	    
	    // 頂点リスト
	    VECTOR posList[2] = {VAdd(pos,downVec),VAdd(pos,upVec)};
	    float vList[2] = {0,1};
	    // 頂点を設定
	    for(int i = 0 ; i < 2; i++){
	    	VERTEX3D vertex;
	    	vertex.pos = posList[i];
	    	vertex.norm = normalVecX;
	    	vertex.dif = GetColorU8(255,0,0,255);
	    	vertex.spc = GetColorU8(0,0,0,0);
	    	vertex.u = 0;
	    	vertex.v = vList[i];
	    	vertex.su = 0;
	    	vertex.sv = 0;
	    	// 頂点を追加
	    	vertex_push_front(vertex);
	    }
	    
	    // 頂点インデックスをずらす
	    for(auto&& index : _vertexIndex){
	        index +=2;
	    }
	    // 頂点インデックスリスト
	    unsigned short indexList[6] = {3,2,1,1,2,0};
	    // 頂点インデックスを追加
	    for(int i = 0; i < 6; i++){
	    	index_push_front(indexList[i]);
	    }

		if(IsStop){
			_animCount = _vertex.size() / 2;
			IsStop = false;
		}

	}
	else{
		IsStop = true;
	}
	// 位置を保存
	_oldPos = pos;
};
//----------------------------------------------------------------------
// @brief 頂点を更新しアニメーションを行う
// @return なし
//----------------------------------------------------------------------
void Afterglow::Process(){
	// u値を更新
	for(auto&& vertex : _vertex){
	    vertex.u += 1.0f / _animCountMax;
		vertex.dif.r -= _subColor;
		vertex.dif.a -= _subColor;
	}

	// 頂点があるとき
	if(_vertex.size() > 0 ){
		// アニメーションカウントが最大値に達していないとき増やす
		if(_animCount < _animCountMax+1){
			_animCount++;
		}
		else{
			// アニメーションカウントが最大値に達しているので削除開始する
			// いらない頂点を削除
			for(int i = 0; i < 2; i++){
			   _vertex.pop_back();
			}
			// いらない頂点インデックスを削除
			for(int i = 0; i < 6; i++){
			   _vertexIndex.pop_back();
			}
		}
		// 頂点がなくなったらアニメーションカウントを0に初期化
		if(_vertex.size() == 0){
			_animCount = 0;
		}
	}

	// 頂点を設定するフラグが立っているとき
	if (IsUpdate) {
		SetVertex();
	}

};
//----------------------------------------------------------------------
// @brief ポリゴンを描画する
// @return なし
//----------------------------------------------------------------------
void Afterglow::Render(){
	if(_vertex.size() >= 4){
		// ライティングとバックカリングを無効にして描画
		SetUseLighting(false);
		SetUseBackCulling(false);
		DrawPolygonIndexed3D(_vertex.data(), _vertex.size(), _vertexIndex.data(), (_vertexIndex.size() / 3.0f)-2, _handle, true);
		// 元に戻す
		SetUseLighting(true);
		SetUseBackCulling(true);
	}
};
