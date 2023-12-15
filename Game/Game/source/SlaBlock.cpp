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
	MV1SetScale(_model, VGet(0.1f, 0.1f, 0.1f));//�����Ă������f�����傫����������1/10�ɐݒ�

	_direction = Math::DegToRad(0);
	_flontAngle = Math::DegToRad(45);//���G�͈͂��Ƃ肠�����S�L��������45�x

	_orignPos = _pos;
	_nextMovePoint = _pos;

	_speed = 5.0f;
	_stopTime = 0.0f;
	_arriveTime = 0;

	_nextDir = 0.0f;
	_oldDir = 0.0f;
	
	_easingFrame = 0;
	_saveNextPoint = VGet(0, 0, 0);

	//���̃��f���ɓ\��t���Ă���e�N�X�`��
	MV1SetTextureGraphHandle(_model, 0, global.resource->LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff2.jpg"), true);
	MV1SetTextureGraphHandle(_model, 1, global.resource->LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff.jpg"), true ) ;
};

SlaBlock::~SlaBlock() {
	MV1DeleteModel(_model);
};

bool SlaBlock::Process() {

	//���G���
	if (_state == TYPE::search) {
		//�ړ������̐ݒ�
		if (StopPos()) {
			if (_stopTime == 0) {
				_stopTime = (float)(rand() % 200) / 100.0f + 1.0f;//1�b����3�b�܂Ŏ~�܂�@�R���}�O.0�P�܂�
				_arriveTime = GetNowCount();
			}

			if ((float)(GetNowCount() - _arriveTime) / 1000 >= _stopTime) {
				if (_nextDir == 0.0f) {

					VECTOR vArrow = VGet((float)(rand() % 20 / 10.0f) - 1.0f, 1.0f, (float)(rand() % 20 / 10.0f) - 1.0f);//�����_���ȕ����x�N�g�������
					vArrow = VScale(vArrow, rand() % (int)_moveRange); vArrow.y = 0.0f;//��_����̔��a���������_���Ŋ|���A���ɐi�ރ|�C���g�����߂�
					_nextMovePoint = VAdd(vArrow, _orignPos);//��_�ɕ��s�ړ�

					//atan2�^�C�v
					//VECTOR dirVec = VSub(_nextMovePoint, _pos);//�����x�N�g�����烂�f���������������v�Z
					//_direction = atan2(dirVec.x, dirVec.z)+180*3.14/180;//-���Ȃ�������Rad��180�𑫂��Ă���

					//�t�H���[�h�x�N�g���^�C�v
					VECTOR dirVec = VSub(_nextMovePoint, _pos);//�����x�N�g�����烂�f���������������v�Z
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
						_stopTime = 0;//���Ԃ̏�����
						_nextMovePoint = _saveNextPoint;
					}
				}
			}
		}
		else {
			//�ړ�����
			VECTOR move = VSub(_nextMovePoint, _pos);
			move = VNorm(move);
			move = VScale(move, _speed);
			_pos = VAdd(_pos, move);
		}
	}

	//���G����
	VECTOR v_length = VSub(_player->GetCollision().down_pos, _pos);
	float len = VSize(v_length);
	if (VSize(v_length) <= _sartchRange) {

		MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		VECTOR pla_dir = VNorm(v_length);
		float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

		if (range_dir <= _flontAngle) {
			_state = TYPE::discover;//��Ԃ𔭌��ɂ���
			_sartchRange = _discoverRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
		}
	}

	//�Ώ۔������
	if (_state == TYPE::discover) {
		//�ړ�����
		VECTOR move = VSub(_player->GetCollision().down_pos, _pos);
		move = VNorm(move);
		move = VScale(move, _speed);
		_pos = VAdd(_pos, move);

		//�ړ������Ɍ�����ς���
		VECTOR dirVec = VScale(move, -1);//�����x�N�g�����烂�f���������������v�Z
		_direction = atan2(dirVec.x, dirVec.z);

		//���G����
		move = VSub(_player->GetCollision().down_pos, _pos);
		float p_distance = VSize(move);//�G�ƃv���C���[�̋���
		if (p_distance >= _sartchRange) {
			_state = TYPE::search;//��Ԃ����G�ɂ���
			_sartchRange = _sartchRangeSize;//���G�͈͂𔭌����̔��a�ɕύX
			_orignPos = _nextMovePoint = _pos;
		}
	}

	//�ŏI�I�ȃ��f���̈ʒu��p�x�𒲐�
	MV1SetRotationXYZ(_model, VGet(0, _direction, 0));
	MV1SetPosition(_model, _pos);
	return true;
}

bool SlaBlock::Render() {

	MV1DrawModel(_model);

	//���G�͈͂Ȃǂ̕`��
	// MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
	// VECTOR now_dir = VScale(Math::MatrixToVector(matrix, 2), -1);//�t�H���[�h�x�N�g��
	////------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	////���E
	//DrawLine3D(_pos, VAdd(_pos, VScale(now_dir, 3000)),GetColor(255,0,0));
	//DrawLine3D(_pos, VAdd(_pos,VTransform( VScale(now_dir, 3000),MGetRotY(45*3.14/180))), GetColor(0, 255, 0));
	//DrawLine3D(_pos, VAdd(_pos, VTransform(VScale(now_dir, 3000), MGetRotY(-45 * 3.14 / 180))), GetColor(0, 255, 0));
	////------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	////���o
	//TDD::ThrDimColOfCircleDraw(_pos, 2000, 0, false);
	////-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	////�U�����̍��G�͈�
	//TDD::ThrDimColOfCircleDraw(_pos, 10000, 0, false);
	////-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//TDD::ThrDimColOfCircleDraw(_orignPos, 1000, 0, false);
	

	return true;
};               