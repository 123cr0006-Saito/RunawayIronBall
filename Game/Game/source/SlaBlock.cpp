#include "SlaBlock.h"

const float SlaBlock::_fixSartchSize = 3000.0f;
const float SlaBlock::_fixDiscoverSize = 10000.0f;
const float SlaBlock::_fixAttackSize = 500.0f;

SlaBlock::SlaBlock(int model, VECTOR pos, Player* Player) :EnemyBase::EnemyBase(model, pos)
{
	//デバック時登録
	_model = MV1LoadModel("res/katatumuri/snail.mv1");
	//今のモデルに貼り付けているテクスチャ
	MV1SetTextureGraphHandle(_model, 0, global.resource->LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff2.jpg"), true);
	MV1SetTextureGraphHandle(_model, 1, global.resource->LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff.jpg"), true);

	_sartchRangeSize = _fixSartchSize;
	_discoverRangeSize = _fixDiscoverSize;
	_attackRangeSize = _fixAttackSize;

	//個別でセットするもの
	_player = Player;
	_sartchRange = _sartchRangeSize;
	_moveRange = 1000.0f;
	_speed = 5.0f;

	MV1SetScale(_model, VGet(0.1f, 0.1f, 0.1f));//持ってきたモデルが大きかったため1/10に設定
};

SlaBlock::~SlaBlock() {
	EnemyBase::~EnemyBase();
};

bool SlaBlock::Process() {
	//基底の索敵状態と発見状態
	EnemyBase::Process();

	//ここから下に攻撃処理を描く
	if (_state == TYPE::attack) {

		//プランナーさん側で変更できる場所
		int enemyToPlayerPosFrame = 30;//敵がプレイヤーの位置につくまでのフレーム
		int fallTime = 60;//落下するまでのフレーム
		float moveCoolTime = 5.0f; //攻撃してからのクールタイム   ※秒数単位 
		if (_currentTime == 0) {
			_easingFrame++;

			if (_easingFrame < enemyToPlayerPosFrame) {
				_pos.x = Easing::InQuart(_easingFrame, _savePos.x, _saveNextPoint.x, enemyToPlayerPosFrame);
				_pos.y = Easing::InQuart(_easingFrame, _savePos.y, _saveNextPoint.y, enemyToPlayerPosFrame);
				_pos.z = Easing::InQuart(_easingFrame, _savePos.z, _saveNextPoint.z, enemyToPlayerPosFrame);
			}

			if (_easingFrame >= fallTime + enemyToPlayerPosFrame) {
				_pos.y -= _speed * 8;
				//とりあえずyが0になるまで落下
				if (_pos.y <= 0.0f) {
					_pos.y = 0.0f;
					global.effect->SetVibration(0, 20, 20);
					_currentTime = GetNowCount();
				}
			}

		}
		else {
			if (GetNowCount() - _currentTime >= moveCoolTime * 1000) {
				_easingFrame = 0;
				_currentTime = 0;
				_state = TYPE::discover;
			}
		}

	}

	//最終的なモデルの位置や角度を調整
	MV1SetRotationXYZ(_model, VGet(0, _direction, 0));
	MV1SetPosition(_model, _pos);
	return true;
}

bool SlaBlock::Render() {
	EnemyBase::Render();
	MV1DrawModel(_model);
	return true;
};