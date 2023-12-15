#include "SlaBlock.h"

const float SlaBlock::_fixSartchSize = 3000.0f;
const float SlaBlock::_fixDiscoverSize = 10000.0f;

SlaBlock::SlaBlock(int model, VECTOR pos, Player* Player) :EnemyBase::EnemyBase(model, pos)
{
	//�f�o�b�N���o�^
	_model = MV1LoadModel("res/katatumuri/snail.mv1");
	//���̃��f���ɓ\��t���Ă���e�N�X�`��
	MV1SetTextureGraphHandle(_model, 0, global.resource->LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff2.jpg"), true);
	MV1SetTextureGraphHandle(_model, 1, global.resource->LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff.jpg"), true);

	_sartchRangeSize = _fixSartchSize;
	_discoverRangeSize = _fixDiscoverSize;

	//�ʂŃZ�b�g�������
	_player = Player;
	_sartchRange = _sartchRangeSize;
	_moveRange = 1000.0f;
	_speed = 5.0f;

	MV1SetScale(_model, VGet(0.1f, 0.1f, 0.1f));//�����Ă������f�����傫����������1/10�ɐݒ�
};

SlaBlock::~SlaBlock() {
	EnemyBase::~EnemyBase();
};

bool SlaBlock::Process() {
	//���̍��G��ԂƔ������
	EnemyBase::Process();
	//�������牺�ɍU��������`��

	//�ŏI�I�ȃ��f���̈ʒu��p�x�𒲐�
	MV1SetRotationXYZ(_model, VGet(0, _direction, 0));
	MV1SetPosition(_model, _pos);
	return true;
}

bool SlaBlock::Render() {
	EnemyBase::Render();
	MV1DrawModel(_model);
	return true;
};