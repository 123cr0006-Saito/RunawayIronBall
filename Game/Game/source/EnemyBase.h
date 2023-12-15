#pragma once
#include "appframe.h"
#include "Player.h"
#include "math.h"
class EnemyBase
{
public:
	EnemyBase(int model, VECTOR pos);
	~EnemyBase();

	virtual bool Process();
	virtual bool Render();

	virtual bool StopPos();

protected:
	Player* _player;
	VECTOR _pos;//エネミーの座標
	float _direction;//y軸の向いている方向
	float _sartchRange;//聴覚範囲の半径
	float _moveRange;//移動範囲の半径
	int    _model;//モデル
	int    _speed;//移動速度
	float _r;//当たり判定用の半径

	float _stopTime;//停止している時間
	int    _arriveTime;//移動地点についた時の時間

	VECTOR _orignPos;//出現地点
	VECTOR _nextMovePoint;//次の移動地点

	static const float _flontAngle;//視界範囲の角度
	float _sartchRangeSize;//索敵範囲の半径
	float _discoverRangeSize;//発見時、対象の見失うまでの距離の半径;

	//回旋させるための変数
	float _nextDir;
	float _oldDir;

	//イージングさせるための変数
	int _easingFrame;
	VECTOR _saveNextPoint;

	enum TYPE : int {
		search,
		discover,
		attack
	};

	TYPE _state;//今の状態

};

