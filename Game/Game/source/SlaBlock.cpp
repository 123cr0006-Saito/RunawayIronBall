#include "SlaBlock.h"

const float SlaBlock::_fixSartchSize = 3000.0f;
const float SlaBlock::_fixDiscoverSize = 10000.0f;
const float SlaBlock::_fixAttackSize = 500.0f;

SlaBlock::SlaBlock(int model, VECTOR pos, Player* Player) :EnemyBase::EnemyBase(model, pos)
{
	//�f�o�b�N���o�^
	_model = MV1LoadModel("res/katatumuri/snail.mv1");
	//���̃��f���ɓ\��t���Ă���e�N�X�`��
	MV1SetTextureGraphHandle(_model, 0, ResourceServer::LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff2.jpg"), true);
	MV1SetTextureGraphHandle(_model, 1, ResourceServer::LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff.jpg"), true);

	_sartchRangeSize = _fixSartchSize;
	_discoverRangeSize = _fixDiscoverSize;
	_attackRangeSize = _fixAttackSize;

	//�ʂŃZ�b�g�������
	_player = Player;
	_sartchRange = _sartchRangeSize;
	_moveRange = 1000.0f;
	_speed = 5.0f;
	//_r = 100.0f;

	MV1SetScale(_model, VGet(0.1f, 0.1f, 0.1f));//�����Ă������f�����傫����������1/10�ɐݒ�
};

SlaBlock::~SlaBlock() {
	EnemyBase::~EnemyBase();
};

bool SlaBlock::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//���̏�ԂɂȂ��Ă��牽�b�o�������H

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	int enemyRigidityTime = 1.0f * 1000; //�U�����[�V�����ɓ����Ă̍d������
	int enemyToPlayerPosFrame = 20.0f / 60.0f * 1000;//�G���v���C���[�̈ʒu�ɂ��܂ł̎���
	int fallTime = 1.0f * 1000;//��������܂ł̎���

	// 1�b�҂��Ă���20�t���[���Ńv���C���[�̓���ɓ���
	if (enemyRigidityTime <= nowTime && nowTime < enemyRigidityTime + enemyToPlayerPosFrame) {
		_pos.x = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.x, _saveNextPoint.x, enemyToPlayerPosFrame);
		_pos.y = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.y, _saveNextPoint.y, enemyToPlayerPosFrame);
		_pos.z = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.z, _saveNextPoint.z, enemyToPlayerPosFrame);
	}

	//�P�b�҂��Ă��痎��
	if (nowTime >= enemyRigidityTime + enemyToPlayerPosFrame + fallTime) {
		_pos.y -= _speed * 8;
		//�Ƃ肠����y��0�ɂȂ�܂ŗ���
		if (_pos.y <= 0.0f) {
			_pos.y = 0.0f;
			_easingFrame = 0;
			//global.effect->SetVibration(0, 20, 20);
			_currentTime = GetNowCount();
			_state = TYPE::cooltime;
		}
	}
	return true;
};

bool SlaBlock::ModeCoolTime() {

	//�v�����i�[���񑤂ŕύX�ł���ꏊ�@���b���P�� 
	float moveCoolTime = 3.0f * 1000; //�U�����Ă���̃N�[���^�C��   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = 0;
		_state = TYPE::discover;
	}
	return true;
};