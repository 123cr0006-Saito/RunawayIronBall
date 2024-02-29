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



	// �f�o�b�O���̕\��
	void DrawDebugInfo();

private:
	// �S���̃��f���n���h��
	int _ibModelHandle;
	// �S���̍��W
	VECTOR _ibPos;

	// ���̃��f���n���h��
	int _chainModelHandle;
	// ���̍��W
	std::vector<VECTOR> _chainPos;

	float _chainDistance;

	// �Y�̍��W
	VECTOR* _stakePos;
};