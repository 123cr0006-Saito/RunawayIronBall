#pragma once
#include "appframe.h"
#include "Player.h"
#include "math.h"
//エネミー各種のもとになるクラス
class EnemyBase
{
public:
	EnemyBase(int model, VECTOR pos);
	~EnemyBase();

	bool Process();
	bool Render();

	virtual bool DebugRender();

	virtual bool ModeSearch();
	virtual bool ModeDisCover();
	virtual bool ModeAttack();
	virtual bool ModeCoolTime();

	virtual bool SetState();

	bool StopPos();

	virtual VECTOR GetCollisionPos() { return VAdd(_pos, _diffeToCenter); }
	float GetR() { return _r; }

protected:
	Player* _player;
	//主な変数
	int    _model;//モデル
	VECTOR _pos;//エネミーの座標
	int    _speed;//移動速度
	float _direction;//y軸の向いている方向
	float _r;//当たり判定用の半径
	VECTOR _diffeToCenter;//コリジョンに使用する中心点までの差分

	//秒数保存用変数
	float _stopTime;//停止している時間
	int    _currentTime;//移動地点についた時の時間

	//座標格納系変数
	VECTOR _orignPos;//出現地点
	VECTOR _nextMovePoint;//次の移動地点
	VECTOR _savePos;//自分の位置を格納

	//索敵系変数
	static const float _flontAngle;//視界範囲の角度
	float _sartchRange;//聴覚範囲の半径
	float _moveRange;//移動範囲の半径
	float _sartchRangeSize;//索敵範囲の半径
	float _discoverRangeSize;//発見時、対象の見失うまでの距離の半径
	float _attackRangeSize;//正面の攻撃範囲

	//回旋させるための変数
	float _nextDir;//回旋するときの次の角度
	float _oldDir;//回旋する前に格納する角度

	//イージングさせるための変数
	int _easingFrame;//イージングのフレーム格納変数
	VECTOR _saveNextPoint;//次に移動する地点

	//敵の状態
	enum TYPE : int {
		search,
		discover,
		attack,
		cooltime
	};

	TYPE _state;//今の状態

};

//敵のような多量のvector配列を持つときのeraseは最後の要素と交換してからerase使用
//処理時間が半減します。
//敵だったら配列の中身がバラバラになっても大丈夫でしょう・・・