#pragma once
#include "appframe.h"
#include "Player.h"
#include "math.h"
class EnemyBase
{
public: 
	EnemyBase();
	~EnemyBase();

	virtual bool Process();
	virtual bool Render();

	virtual bool StopPos();

	virtual void CollisionDetection();

protected :
	VECTOR _pos;//エネミーの座標
	float _direction;//y軸の向いている方向
	float _sartchRange;//聴覚範囲の半径
	float _flontAngle;//視界範囲の角度
	float _moveRange;//移動範囲の半径
	int _model;//モデル
	int _speed;//移動速度
	float _r;//半径

	float _stopTime;//停止している時間
	int _arriveTime;//移動地点についた時の時間

	VECTOR _orignPos;//出現地点
	VECTOR _nextMovePoint;//次の移動地点


	enum TYPE : int {
		search,
		discover
	};

	TYPE _state;//今の状態

};

