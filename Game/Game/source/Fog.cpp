//----------------------------------------------------------------------
// @filename Fog.cpp
// @date: 2024/02/22
// @author: saito ko
// @explanation
// プレイヤーが決められた範囲外に出るとフォグが発生するクラス
//----------------------------------------------------------------------
#include "Fog.h"
#include "modegame.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
Fog::Fog(){
	_player = Player::GetInstance();
	SetFogColor(255,255,255);
	_fogEndDistance = _fogDistanceMax;
	_param = _fogDistanceMax;
	_easingCount = 0;
	IsFog= false;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
Fog::~Fog(){
	_player = nullptr;
	if (IsFog) {
		SetFogEnable(false);
	}
};
//----------------------------------------------------------------------
// @brief フォグを行うかどうかを設定
// @param stageNum ステージ番号
// @return 無し
//----------------------------------------------------------------------
void Fog::UpdateIsFog(int stageNum){
	
	VECTOR pos = _player->GetPosition();
	float length = VSquareSize(pos);

	float stage_length[3] = { STAGE_ONE_WIDTH ,STAGE_TWO_WIDTH,STAGE_THREE_WIDTH };
	float fogLength = stage_length[stageNum-1] - 400.0f;
	if (length > fogLength * fogLength) {
		if(IsFog == false){
		IsFog = true;
		SetFogEnable(IsFog);
		_fogEndDistance = _fogDistanceMin;
		_easingCount = 0;
		}
	}
	else {
		if(IsFog == true){
		   IsFog = false;
		   _fogEndDistance = _fogDistanceMax;
		   _easingCount = 0;
		}
	}
};
//----------------------------------------------------------------------
// @brief フォグの処理
// @param stageNum ステージ番号
// @return 無し
//----------------------------------------------------------------------
void Fog::Process(int stageNum){
	
	UpdateIsFog(stageNum);
	int easingTime = 60;

	if(_easingCount < easingTime){
		_easingCount++;
		_param = Easing::Linear(_easingCount, _param, _fogEndDistance, easingTime);
		SetFogStartEnd(1000, _param);
		if(!IsFog && _easingCount >= easingTime){
			SetFogEnable(IsFog);
		}
	}

};