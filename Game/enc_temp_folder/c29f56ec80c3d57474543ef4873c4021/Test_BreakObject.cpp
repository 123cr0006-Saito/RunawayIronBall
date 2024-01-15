#include "Test_BreakObject.h"

BreakObject::BreakObject()
{
	_modelHandle = MV1LoadModel("res/Test_BreakObject/centerpivot_2.mv1");
	_pos = VGet(0, 0, 0);
	MV1SetPosition(_modelHandle, _pos);
	//MV1SetScale(_modelHandle, VGet(10.0f, 10.0f, 10.0f));

	//_breakFrameIndex = 35;

	_breakCnt = 0;

	_blastDir = VGet(0.0f, 0.0f, -1.0f);
	_blastPower = 10.0f;


	_isDrawLocus = false;
}

BreakObject::~BreakObject()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
	}
}

void BreakObject::Init()
{
	for (int i = 0; i < MV1GetFrameNum(_modelHandle); i++) {
		// �t���[���̍��W�ϊ��s����擾����
		MATRIX mFrameLocal = MV1GetFrameLocalMatrix(_modelHandle, i);
		// �t���[���̃��[�J�����W�����߂�
		VECTOR vFrameLocalPos = VGet(0.0f, 0.0f, 0.0f);
		vFrameLocalPos = VTransform(vFrameLocalPos, mFrameLocal);
	
		// ���f���̋N�_���W���猩���t���[���̕������擾����
		VECTOR vFrameLocalDir = VGet(0.0f, 0.0f, -1.0f);
		if (VSquareSize(vFrameLocalPos) > 0.0000f) {
			vFrameLocalDir = VNorm(vFrameLocalPos);
			// ������ԕ�����z���}�C�i�X�����Ɍ��肷��
			if (vFrameLocalDir.z > 0.0f) {
				vFrameLocalDir.z *= -1;
			}
		}

		FRAME_INFO f = { i, vFrameLocalDir };
		_frameInfo.push_back(f);

		_locus.resize(_locus.size() + 1);
	}






	// �f�o�b�O�p
	
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, itr->frameIndex);

		VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
		itr->pos = itr->startPos = v;

		_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(v);
	}
	
	// ������΂��������w��
	SetBlastDir(VGet(1.0f, 0.0f, 0.0f));
}

void BreakObject::Process()
{
	// �j�Ђ���юU�鏈��
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MATRIX mRot = MGetRotX(itr->rotVector.x);
		mRot = MMult(mRot, MGetRotY(itr->rotVector.y));
		mRot = MMult(mRot, MGetRotZ(itr->rotVector.z));

		// �ړ��O�̍s��
		MATRIX mBefor = MV1GetFrameLocalMatrix(_modelHandle, itr->frameIndex);
		// ���s�ړ��s��i���������Ɖ��������̕��s�ړ��s�����������j
		MATRIX mTrans = MGetTranslate(VAdd(VScale(itr->horizontalDir, itr->horizontalVelocity), VGet(0.0f, itr->verticalVelocity, 0.0f)));
		
		MATRIX m = MMult(mRot, mBefor);
		m = MMult(m, mTrans);

		// �s��̓K��
		MV1SetFrameUserLocalMatrix(_modelHandle, itr->frameIndex, m);



		// �d�͏���
		itr->verticalVelocity -= 2.0f;

		// �O�Օ\���p�̍��W��ێ�
		MATRIX mLocus = MV1GetFrameLocalWorldMatrix(_modelHandle, itr->frameIndex);
		VECTOR vLocus = VTransform(VGet(0.0f, 0.0f, 0.0f), mLocus);
		_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(vLocus);
		//itr->pos = v;
	}
	//_blastPower -= 0.5f;

	_breakCnt++;
	//_blastDir.y -= 0.05f;

	 // ���Z�b�g
	if (_breakCnt > 90) {
		_breakCnt = 0;

		// ������΂��������w��
		SetBlastDir(VGet(0.0f, 0.0f, 1.0f));

		// �O�Օ\���p�̍��W�������Z�b�g
		for (auto itr = _locus.begin(); itr != _locus.end(); ++itr) {
			itr->clear();
		}

		// �p�[�c�������ʒu�ɖ߂�
		for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
			MV1ResetFrameUserLocalMatrix(_modelHandle, itr->frameIndex);

			MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, itr->frameIndex);
			VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
			_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(v);
		}
	}

	// �O�Օ\����On/Off�؂�ւ�
	auto input = XInput::GetInstance();
	if (input->GetTrg(XINPUT_BUTTON_BACK) != 0) {
		_isDrawLocus = !_isDrawLocus;
	}
}

void BreakObject::Render()
{
	MV1SetPosition(_modelHandle, _pos);
	MV1DrawModel(_modelHandle);
}

// ������΂��̕������Z�b�g����
// ����
// vDir : �ӂ���΂��̒��S����
void BreakObject::SetBlastDir(VECTOR vDir)
{
	vDir = VNorm(vDir);
	// �p�[�c���Ƃɐ�����΂�����������vDir���� �}maxRange�x�̊ԂŃ����_���Ɍ��肷��
	const int maxRange = 45;
	// �����E���������ɂ�����ő呬�x
	const int maxVelocity = 30;

	// �p�[�c���Ƃɂӂ���΂��������Z�b�g����
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		// ��������
		float angle = rand() % (maxRange * 2);
		angle -= maxRange;
		itr->horizontalDir = VTransform(vDir, MGetRotY(Math::DegToRad(angle)));
		itr->horizontalVelocity = rand() % maxVelocity;

		// ��������
		itr->verticalVelocity = (rand() % maxVelocity) + 20.0f;

		int deltaRot = 8;
		float angleX = rand() % deltaRot;
		angleX = Math::DegToRad(angleX);
		float angleY = rand() % deltaRot;
		angleY = Math::DegToRad(angleY);
		float angleZ = rand() % deltaRot;
		angleZ = Math::DegToRad(angleZ);
		itr->rotVector = VGet(angleX, angleY, angleZ);
	}
	//_blastPower = 20.0f;
	_blastDir = vDir;
}

void BreakObject::DrawDebugInfo()
{
	if (_isDrawLocus) {
		// �ӂ���΂������̒��S
		{
			VECTOR startPos = VGet(0.0f, 0.0f, 0.0f);
			DrawLine3D(startPos, VAdd(startPos, VScale(_blastDir, 1000.0f)), GetColor(255, 255, 255));
		}

		// �p�[�c���Ƃɐ�����т̋O�Ղ�\������
		for (auto i = _locus.begin(); i != _locus.end(); ++i) {
			for (auto j = i->begin(); j != i->end() - 1; ++j) {
				DrawLine3D((*j), (*(j + 1)), GetColor(255, 255, 0));
			}
		}
	}
}