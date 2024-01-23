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
	float _sartchRange;//索敵範囲の半径
	float _discoverRangeSize;//発見時、対象の見失うまでの距離の半径
	float _attackRangeSize;//正面の攻撃範囲
}EnemyParam;