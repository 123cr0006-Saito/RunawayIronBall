//----------------------------------------------------------------------
// @filename EnemyStract.h
// @date: 2024/01/26
// @author: saito ko
// @explanation
// エネミーで使用する構造体を定義するヘッダーです。
//----------------------------------------------------------------------
#pragma once
//Enemyクラスを管理するPoolとEnemy本体であるBaseで同じ構造体を使用するために作ったヘッダーです。
//他の方法を思いついたらこのヘッダーを削除すると思います。

typedef struct {
	//敵判別用の名前
	int _hp;//敵の体力
	int _exp;//敵から得られる経験値
	float  _speed;//移動速度
	int _coolTime;//攻撃後のクールタイム
	//索敵系変数
	float _flontAngle;//視界範囲の角度
	float _hearingRangeSize;//聴覚範囲の半径
	float _moveRange;//移動範囲の半径
	float _searchRange;//索敵範囲の半径
	float _discoverRangeSize;//発見時、対象の見失うまでの距離の半径
	float _attackRangeSize;//正面の攻撃範囲
	int _suppression;//制圧値
}EnemyParam;

//敵の状態
enum class ENEMYTYPE : int {
	SEARCH,
	DISCOVER,
	ATTACK,
	COOLTIME,
	KNOCKBACK,
	DEAD
};

//索敵の種類
enum class SEARCHTYPE : int {
	COOLTIME,
	TURN,
	MOVE
};