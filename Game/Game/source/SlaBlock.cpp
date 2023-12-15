#include "SlaBlock.h"

const float SlaBlock::_sartchRangeSize = 3000.0f;
const float SlaBlock::_discoverRangeSize = 10000.0f;

SlaBlock::SlaBlock(std::string model_name,VECTOR pos,Player* Player) :
	_player(Player)
{
	EnemyBase::EnemyBase();
	_sartchRange = _sartchRangeSize;
	_moveRange = 1000.0f;
	_model = MV1LoadModel(model_name.c_str());
	_pos = pos;
	MV1SetScale(_model, VGet(0.1f, 0.1f, 0.1f));//持ってきたモデルが大きかったため1/10に設定

	_direction = Math::DegToRad(0);
	_flontAngle = Math::DegToRad(45);//索敵範囲をとりあえず全キャラ統一45度

	_orignPos = _pos;
	_nextMovePoint = _pos;

	_speed = 5.0f;
	_stopTime = 0.0f;
	_arriveTime = 0;

	_nextDir = 0.0f;
	_oldDir = 0.0f;
	
	_easingFrame = 0;
	_saveNextPoint = VGet(0, 0, 0);

	//今のモデルに貼り付けているテクスチャ
	MV1SetTextureGraphHandle(_model, 0, global.resource->LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff2.jpg"), true);
	MV1SetTextureGraphHandle(_model, 1, global.resource->LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff.jpg"), true ) ;
};

SlaBlock::~SlaBlock() {
	MV1DeleteModel(_model);
};

bool SlaBlock::Process() {

	//索敵状態
	if (_state == TYPE::search) {
		//移動方向の設定
		if (StopPos()) {
			if (_stopTime == 0) {
				_stopTime = (float)(rand() % 200) / 100.0f + 1.0f;//1秒から3秒まで止まる　コンマ０.0１まで
				_arriveTime = GetNowCount();
			}

			if ((float)(GetNowCount() - _arriveTime) / 1000 >= _stopTime) {
				if (_nextDir == 0.0f) {

					VECTOR vArrow = VGet((float)(rand() % 20 / 10.0f) - 1.0f, 1.0f, (float)(rand() % 20 / 10.0f) - 1.0f);//ランダムな方向ベクトルを取る
					vArrow = VScale(vArrow, rand() % (int)_moveRange); vArrow.y = 0.0f;//基準点からの半径分をランダムで掛け、次に進むポイントを決める
					_nextMovePoint = VAdd(vArrow, _orignPos);//基準点に平行移動

					//atan2タイプ
					//VECTOR dirVec = VSub(_nextMovePoint, _pos);//方向ベクトルからモデルが向く方向を計算
					//_direction = atan2(dirVec.x, dirVec.z)+180*3.14/180;//-をなくすためRadの180を足している

					//フォワードベクトルタイプ
					VECTOR dirVec = VSub(_nextMovePoint, _pos);//方向ベクトルからモデルが向く方向を計算
					dirVec = VNorm(dirVec);
					MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
					VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
					float range_dir = Math::CalcVectorAngle(ene_dir, dirVec);
					 VECTOR arrow = VCross(ene_dir, dirVec);
					if (arrow.y < 0) {
						range_dir *= -1;
					}
					_nextDir = _direction + range_dir;
					_oldDir = _direction;
				}
				else {
					_easingFrame++;
					_direction = Easing::Linear(_easingFrame, _oldDir, _nextDir, 60);
					if (_easingFrame >= 60) {
						_easingFrame = 0;
						_nextDir = 0.0f;
						_stopTime = 0;//時間の初期化
						_nextMovePoint = _saveNextPoint;
					}
				}
			}
		}
		else {
			//移動処理
			VECTOR move = VSub(_nextMovePoint, _pos);
			move = VNorm(move);
			move = VScale(move, _speed);
			_pos = VAdd(_pos, move);
		}
	}

	//索敵処理
	VECTOR v_length = VSub(_player->GetCollision().down_pos, _pos);
	float len = VSize(v_length);
	if (VSize(v_length) <= _sartchRange) {

		MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		VECTOR pla_dir = VNorm(v_length);
		float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

		if (range_dir <= _flontAngle) {
			_state = TYPE::discover;//状態を発見にする
			_sartchRange = _discoverRangeSize;//索敵範囲を発見時の半径に変更
		}
	}

	//対象発見状態
	if (_state == TYPE::discover) {
		//移動処理
		VECTOR move = VSub(_player->GetCollision().down_pos, _pos);
		move = VNorm(move);
		move = VScale(move, _speed);
		_pos = VAdd(_pos, move);

		//移動方向に向きを変える
		VECTOR dirVec = VScale(move, -1);//方向ベクトルからモデルが向く方向を計算
		_direction = atan2(dirVec.x, dirVec.z);

		//索敵処理
		move = VSub(_player->GetCollision().down_pos, _pos);
		float p_distance = VSize(move);//敵とプレイヤーの距離
		if (p_distance >= _sartchRange) {
			_state = TYPE::search;//状態を索敵にする
			_sartchRange = _sartchRangeSize;//索敵範囲を発見時の半径に変更
			_orignPos = _nextMovePoint = _pos;
		}
	}

	//最終的なモデルの位置や角度を調整
	MV1SetRotationXYZ(_model, VGet(0, _direction, 0));
	MV1SetPosition(_model, _pos);
	return true;
}

bool SlaBlock::Render() {

	MV1DrawModel(_model);

	//索敵範囲などの描画
	// MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
	// VECTOR now_dir = VScale(Math::MatrixToVector(matrix, 2), -1);//フォワードベクトル
	////------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	////視界
	//DrawLine3D(_pos, VAdd(_pos, VScale(now_dir, 3000)),GetColor(255,0,0));
	//DrawLine3D(_pos, VAdd(_pos,VTransform( VScale(now_dir, 3000),MGetRotY(45*3.14/180))), GetColor(0, 255, 0));
	//DrawLine3D(_pos, VAdd(_pos, VTransform(VScale(now_dir, 3000), MGetRotY(-45 * 3.14 / 180))), GetColor(0, 255, 0));
	////------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	////聴覚
	//TDD::ThrDimColOfCircleDraw(_pos, 2000, 0, false);
	////-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	////攻撃時の索敵範囲
	//TDD::ThrDimColOfCircleDraw(_pos, 10000, 0, false);
	////-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//TDD::ThrDimColOfCircleDraw(_orignPos, 1000, 0, false);
	

	return true;
};               