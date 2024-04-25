//----------------------------------------------------------------------
// @filename EffekseerBase.cpp
// ＠date: 2024/02/8
// ＠author: saito ko
// @explanation
// エフェクシアを再生するための基本的なクラス
//----------------------------------------------------------------------
#include "EffekseerBase.h"

//エフェクシアの関数でUpdateEffekseer3Dと描画関係の関数は
//1Ｆに１回だけ呼び出す
//何回も呼び出すとそのぶん一緒に処理される
//----------------------------------------------------------------------
// @breif:コンストラクタ
// @param: name エフェクトのファイルパス
// @param: pos エフェクトの座標
// @param: size エフェクトの倍率
// @param: height 基準点からずらす高さ
// @param: speed 再生速度
// @param: loopFlag ループするかどうか
// @param: loadFlag 読み込みをするかどうか
// @return:なし
//----------------------------------------------------------------------
EffekseerBase::EffekseerBase(std::string name, VECTOR* pos,float size, float height,float speed,bool loopFlag, bool loadFlag) :
	EffectBase(),
	_effectResourceHandle(ResourceServer::LoadEffekseerEffect(_T(name.c_str()),_T(name.c_str()))),
	_pos(*pos),
	_speed(speed),
	_size(size),
	_loopFlag(loopFlag),
	_height(height)
{
	if (loadFlag) {
		_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
		SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
		SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
	}
	_height = 0.0f;
};
//----------------------------------------------------------------------
// @breif:コンストラクタ
// @param: handle エフェクトのハンドル
// @param: pos エフェクトの座標
// @param: size エフェクトの倍率
// @param: height 基準点からずらす高さ
// @param: speed 再生速度
// @param: loopFlag ループするかどうか
// @param: loadFlag 読み込みをするかどうか
// @return:なし
//----------------------------------------------------------------------
EffekseerBase::EffekseerBase(int handle, VECTOR* pos, float size, float height, float speed , bool loopFlag, bool loadFlag) :
	EffectBase(),
	_effectResourceHandle(handle),
	_pos(*pos),
	_speed(speed),
	_size(size),
	_loopFlag(loopFlag),
	_height(height)
{
	if (loadFlag) {
		_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
		SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
		SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
	}
};
//----------------------------------------------------------------------
// @breif:デストラクタ
// @return:なし
//----------------------------------------------------------------------
EffekseerBase::~EffekseerBase() {
	StopEffekseer3DEffect(_playingEffectHandle);
};
//----------------------------------------------------------------------
// @breif:更新処理
// @return:成功したかどうか
//----------------------------------------------------------------------
bool EffekseerBase::Process() {

	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, _pos.x, _pos.y, _pos.z);

	if (IsEffekseer3DEffectPlaying(_playingEffectHandle) == -1) {
		if (_loopFlag) {
			_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
			SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
			SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
		}
		else {
			StopEffekseer3DEffect(_playingEffectHandle);
			_IsPlay = false;
		}
	}

	return true;
};
//----------------------------------------------------------------------
// @breif:描画処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EffekseerBase::Render() {
	return true;
};