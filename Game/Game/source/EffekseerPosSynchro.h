#pragma once
#include "EffekseerBase.h"
//座標が変化する effekseerクラス
class EffekseerPosSynchro : public EffekseerBase
{
	typedef EffekseerBase base;
public :
	EffekseerPosSynchro(std::string name, VECTOR* pos, float size, float speed = 1.0f, bool loopFlag = false);
	~EffekseerPosSynchro();

	virtual bool Process()override;
private :
	const VECTOR* const _pos; //ポインタの先の値の書き換えを禁止 変数の再代入も不可
};

