//----------------------------------------------------------------------
// @filename EffekseerPosSynchro.h
// ＠date: 2024/02/8
// ＠author: saito ko
// @explanation
// 位置情報のあるアドレスを受け取り移動するエフェクシアを再生するクラス
//----------------------------------------------------------------------
#include "EffekseerPosSynchro.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param name 読み込み時のファイル名
// @param pos エフェクトの再生位置
// @param size エフェクトの拡大倍率
// @param rotation エフェクトの回転
// @param height エフェクトの位置から+y軸方向にずらす高さ
// @param speed エフェクトの再生速度
// @param loopFlag ループするかどうか
// @return 無し
//----------------------------------------------------------------------
EffekseerPosSynchro::EffekseerPosSynchro(std::string name, VECTOR* pos, float size, VECTOR rotation, float height, float speed, bool loopFlag) :
	base(name, pos, size, height, speed, loopFlag, false),
	_pos(pos)
{
	_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
	SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
	SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);

	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	rotation.y = 0.0f;
	float angle = Math::CalcVectorAngle(vBase, rotation);
	angle *= rotation.x < 0.0f ? 1.0f : -1.0f;
	SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, 0, angle, 0);
};
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param handle 読み込み時のファイル名
// @param pos エフェクトの再生位置
// @param size エフェクトの拡大倍率
// @param rotation エフェクトの回転
// @param height エフェクトの位置から+y軸方向にずらす高さ
// @param speed エフェクトの再生速度
// @param loopFlag ループするかどうか
// @return 無し
//----------------------------------------------------------------------
EffekseerPosSynchro::EffekseerPosSynchro(int handle, VECTOR* pos, float size, VECTOR rotation, float height, float speed, bool loopFlag) :
	base(handle, pos, size, height, speed, loopFlag, false),
_pos(pos)
{
	_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
	SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
	SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);

	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	rotation.y = 0.0f;
	float angle = Math::CalcVectorAngle(vBase, rotation);
	angle *= rotation.x < 0.0f ? 1.0f : -1.0f;
	SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, 0, angle, 0);
	
	_currentTime = GetNowCount();
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
EffekseerPosSynchro::~EffekseerPosSynchro() {
	if (IsEffekseer3DEffectPlaying(_playingEffectHandle) != -1) {
		StopEffekseer3DEffect(_playingEffectHandle);
	}
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool EffekseerPosSynchro::Process() {
	float endTime = 50.0f / 60.0f * 1000;
	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, (*_pos).x, (*_pos).y + _height, (*_pos).z);

	if (GetNowCount() - _currentTime >= endTime) {
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
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool EffekseerPosSynchro::Render() {
	return true;
};