//----------------------------------------------------------------------
// @filename Boss.h
// @date: 2024/03/20
// @author: Morozumi Hiroya
// @explanation
// �{�X�̐�����s���N���X
// �{�X�S���̐���́ABossIronBall�N���X�ōs��
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "BossIronBall.h"
#include "Player.h"

namespace {
	// �Y�̍ő�HP
	constexpr int STAKE_MAX_HP = 100;
}
class Boss
{
public:
	Boss();
	~Boss();

	// ���f���̓ǂݍ���
	void LoadModel();
	// ����������
	void Init(VECTOR polePos);
	// �X�V����
	void Process();
	// �`�揈��
	void Render();

	// �Y�̍��W���擾
	VECTOR GetStakePosition() { return _stakePos; }
	// �{�X�S���̍��W���擾
	VECTOR GetIBPosition() { return _ironBall->GetPosition(); }
	// �{�X�S���̍��W��ݒ�
	void SetIBPosition(VECTOR pos) { _ironBall->SetPosition(pos); }

	// �K���X����Ԃ��ǂ������擾����
	bool GetIsGlass() { return _ironBall->GetIsGlass(); }

	// �Y�����G��Ԃ��ǂ������擾����
	bool GetIsStakeInvincible() { return _isStakeInvincible; }

	// �Y�̓����蔻����擾����
	Capsule GetStakeCollision() { return _stakeCapsuleCol; }
	// �{�X�S���̓����蔻����擾����
	Sphere GetIBCollision() { return _ironBall->GetIBCollision(); }
	// �{�X�S���̍��̓����蔻����擾����
	Capsule GetChainCollision() { return _ironBall->GetChainCollision(); }

	// �{�X�S������]�U�����ǂ������擾����
	bool GetIsRotationAttack() { return _ironBall->GetIsRotationAttack(); }

	// �{�X�S�����X�e�[�W��ɂ��邩�ǂ�����ݒ肷��
	void SetOnStage(bool isOnStage) { _ironBall->SetOnStage(isOnStage); }
	// �{�X�S�����X�e�[�W��ɂ��邩�ǂ������擾����
	bool GetOnStage() { return _ironBall->GetOnStage(); }

	// �{�X�S���̖��G��Ԃ��ǂ������擾����
	bool GetIBInvincible() { return _ironBall->GetIsInvincible(); }

	// �{�X�S���ƍY�̓����蔻����s�� 
	void CheckHitBossAndStake();
	// �{�X�S�����m�b�N�o�b�N��Ԃɂ���
	void SetIBKnockBack(VECTOR vDir, float speed) { _ironBall->SetKnockBack(vDir, speed); }
	// �Y��HP�����炷
	void SetDamageStake(int damage);

	// �Y��HP���擾
	int GetStakeHp() { return _stakeHp; }
	// �Y�̍ő�HP���擾
	int GetStakeMaxHp() { return STAKE_MAX_HP; }

	// �Y���j�󂳂ꂽ���ǂ������擾
	bool GetIsStakeBroken() { return _isStakeBroken; }

	// �X�e�[�W�̔��a��ݒ�
	void SetStageRadius(float radius) { _ironBall->SetStageRadius(radius); }

	// �f�o�b�O���̕\��
	void DrawDebugInfo();

private:
	// �Y�̃��f���n���h��
	int _stakeModelHandle;
	// �Y�̍��W
	VECTOR _stakePos;
	// �Y�̓����蔻��
	Capsule _stakeCapsuleCol;
	// �Y��HP
	int _stakeHp;
	// �Y�����G��Ԃ��ǂ���
	bool _isStakeInvincible;
	// �Y�̖��G����
	int _stakeInvincibleCnt;
	// �Y���j�󂳂ꂽ���ǂ���
	bool _isStakeBroken;

	// �S��
	BossIronBall* _ironBall;

	// �v���C���[
	Player* _player;
};