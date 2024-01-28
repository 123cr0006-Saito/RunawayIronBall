#pragma once
#include "appframe.h"
#include "Player.h"
#include "math.h"
#include "EnemyStract.h"

//エネミー各種のもとになるクラス
class EnemyBase
{
public:
	EnemyBase();
	~EnemyBase();

	bool Create(int model, VECTOR pos, EnemyParam param);
	virtual void Init(VECTOR pos, float scale);
	virtual void Init(VECTOR pos);
	virtual void InheritanceInit();
	//---------------------------------------------------------
	//デバッグ用の関数です。素材が来たら後で消します
	void  DebugSnail();
	//---------------------------------------------------------
	void SetPos(VECTOR pos);

	bool Process();
	bool Render();

	virtual bool DebugRender();

	virtual bool ModeSearch();
	virtual bool ModeDisCover();
	virtual bool ModeAttack();
	virtual bool ModeCoolTime();
	virtual bool ModeKnockBack();
	virtual bool ModeDead();

	virtual bool SetState();

	bool StopPos();

	void SetKnockBack(VECTOR vDir, float damage);//攻撃を受けた時の処理

	bool GetUse() { return _IsUse; }
	virtual VECTOR GetCollisionPos() { return VAdd(_pos, _diffeToCenter); }
	float GetR() { return _r; }

protected:
	Player* _player;
	bool _IsUse;

	//Jsonで読み込むもの------------------------------------------------------------------------

	int _hp;//敵の体力
	int _exp;//敵から得られる経験値
	float  _speed;//移動速度
	int _coolTime;//攻撃後のクールタイム

	//索敵系変数
	float _flontAngle;//視界範囲の角度
	float _sartchRange;//索敵範囲の半径
	float _moveRange;//移動範囲の半径
	float _hearingRangeSize;//聴覚範囲の半径
	float _discoverRangeSize;//発見時、対象の見失うまでの距離の半径
	float _attackRangeSize;//正面の攻撃範囲

	//------------------------------------------------------------------------------------------------
	
	//主な変数
	int    _model;//モデル
	VECTOR _pos;//エネミーの座標
	float _gravity;//重力加速度
	float _direction;//y軸の向いている方向
	float _r;//当たり判定用の半径
	float _scale;//敵のサイズ
	VECTOR _diffeToCenter;//コリジョンに使用する中心点までの差分

	//秒数保存用変数
	float _stopTime;//停止している時間
	int    _currentTime;//移動地点についた時の時間

	//座標格納系変数
	VECTOR _orignPos;//出現地点
	VECTOR _nextMovePoint;//次の移動地点
	VECTOR _savePos;//自分の位置を格納

	//回旋させるための変数
	float _nextDir;//回旋するときの次の角度
	float _oldDir;//回旋する前に格納する角度

	//イージングさせるための変数
	int _easingFrame;//イージングのフレーム格納変数
	VECTOR _saveNextPoint;//次に移動する地点

	//ノックバックをするときに移動する
	VECTOR _knockBackDir;//エネミーが攻撃されたとき移動していく方向ベクトル
	int _knockBackSpeedFrame;//エネミーが攻撃されたときに移動するspeedとフレーム

	//敵の状態
	enum class ENEMYTYPE : int {
		SEARCH,
		DISCOVER,
		ATTACK,
		COOLTIME,
		KNOCKBACK,
		DEAD
	};

	ENEMYTYPE _state;//今の状態

};

//敵のような多量のvector配列を持つときのeraseは最後の要素と交換してからerase使用
//処理時間が半減します。
//敵だったら配列の中身がバラバラになっても大丈夫でしょう・・・