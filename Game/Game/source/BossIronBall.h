#pragma once
#include "appframe.h"

class BossIronBall
{
public:
	BossIronBall();
	~BossIronBall();

	void LoadModel();
	void Init(VECTOR* stakePos);

	void Process();
	void Render();


	void UpdateIBCollision();

	// �f�o�b�O���̕\��
	void DrawDebugInfo();


private:
	void CheckState();




	// �ҋ@��Ԃ̏���
	void IdleProcess();

	// �d����Ԃ̏���
	void StiffenProcess();
	void SetStiffen(int cnt) { _ibState = IB_STATE::IB_STATE_STIFFEN; _ibStiffenCnt = cnt; }

	// ��]�U��
	void RotationProcess();
	void RotationAcceleration();

private:
	// �S���̃��f���n���h��
	int _ibModelHandle;
	// �S���̍��W
	VECTOR _ibPos;
	// �S���̓����蔻��
	Sphere _ibSphereCol;

	enum class IB_STATE {
		IB_STATE_IDLE,
		IB_STATE_STIFFEN,
		IB_STATE_ATTACK_RUSH,
		IB_STATE_ATTACK_DROP,
		IB_STATE_ATTACK_ROTATION,
	} _ibState;

	//
	int _ibIdleCnt;
	VECTOR _ibMoveDir;

	int _ibStiffenCnt;



	// 
	bool _activeRotationAcceleration;
	int _rotationAccelerationCnt;
	float _rotAngularVelocity;
	float _rotAngle;

	// ���̃��f���n���h��
	int _chainModelHandle;
	// ���̍��W
	std::vector<VECTOR> _chainPos;

	float _chainDistance;

	// �Y�̍��W
	VECTOR* _stakePos;

	// ���_����Y�̍��W�ւ̕��s�ړ��s��
	MATRIX _mStakePos;
	// _mStakePos�̋t�s��
	MATRIX _mStakePosInv;






	// �f�o�b�O�p
	// ����̃A�N�V�����̃t���[����
	int debugFrame = 30;
	int debugFrameMax = 30;

	float debugValue = 100.0f;
	float debugValueMax = 100.0f;
};