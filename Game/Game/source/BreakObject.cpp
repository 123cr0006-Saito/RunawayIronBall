#include "BreakObject.h"

BreakObject::BreakObject()
{
	_isActive = false;
	_finishedBreakAll = false;
	

	_modelHandle = -1;
	//MV1SetPosition(_modelHandle, _pos);
	//MV1SetScale(_modelHandle, VGet(10.0f, 10.0f, 10.0f));

	//_breakFrameIndex = 35;

	_breakCnt = 0;

	_blastDir = VGet(0.0f, 0.0f, -1.0f);


	_isDrawLocus = false;
}

BreakObject::~BreakObject()
{
	for (auto&& frame : _frameInfo) {
		delete frame; frame = nullptr;
	}
}

void BreakObject::Init(int modelHandle)
{
	_modelHandle = modelHandle;
	for (int i = 0; i < MV1GetFrameNum(_modelHandle); i++) {

		// �t���[���̖��O���擾����
		std::string frameName = MV1GetFrameName(_modelHandle, i);
		std::string checkName = "BRK";
		// �t���[������BRK�Ŏn�܂�Ȃ��ꍇ��continue
		if (frameName.substr(0, 3) != checkName) {
			continue;
		}


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

		FRAME_INFO* f = NEW FRAME_INFO();
		f->frameIndex = i;
		f->finishedBreak;
		f->horizontalDir = vFrameLocalDir;
		f->horizontalVelocity = 0.0f;
		f->verticalVelocity = 0.0f;
		f->vRot = VGet(0.0f, 0.0f, 0.0f);

		_frameInfo.push_back(f);





		_locus.resize(_locus.size() + 1);
	}






	// �f�o�b�O�p

	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, (*itr)->frameIndex);

		VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
		_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(v);
	}

	// ������΂��������w��
	SetBlastDir(VGet(1.0f, 0.0f, 0.0f));
}

void BreakObject::Process()
{
	if (!_finishedBreakAll && _isActive) {
		_finishedBreakAll = true;
		// �j�Ђ���юU�鏈��
		for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {

			if((*itr)->finishedBreak) continue;
			_finishedBreakAll = false;

			// ��]�s��
			MATRIX mRot = MGetRotX((*itr)->vRot.x);
			mRot = MMult(mRot, MGetRotY((*itr)->vRot.y));
			mRot = MMult(mRot, MGetRotZ((*itr)->vRot.z));

			// �ړ��O�̍s��
			MATRIX mBefor = MV1GetFrameLocalMatrix(_modelHandle, (*itr)->frameIndex);
			// ���s�ړ��s��i���������Ɖ��������̕��s�ړ��s�����������j
			MATRIX mTrans = MGetTranslate(VAdd(VScale((*itr)->horizontalDir, (*itr)->horizontalVelocity), VGet(0.0f, (*itr)->verticalVelocity, 0.0f)));

			MATRIX m = MMult(mRot, mBefor);
			m = MMult(m, mTrans);

			// �s��̓K��
			MV1SetFrameUserLocalMatrix(_modelHandle, (*itr)->frameIndex, m);



			// �d�͏���
			(*itr)->verticalVelocity -= 2.f;

			// �O�Օ\���p�̍��W��ێ�
			MATRIX mLocus = MV1GetFrameLocalWorldMatrix(_modelHandle, (*itr)->frameIndex);
			VECTOR vLocus = VTransform(VGet(0.0f, 0.0f, 0.0f), mLocus);
			_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(vLocus);

			if (vLocus.y < 0.0f) {
				(*itr)->finishedBreak = true;
			}

			//itr->pos = v;
		}
		//_blastPower -= 0.5f;

		_breakCnt++;
		//_blastDir.y -= 0.05f;

		// // ���Z�b�g
		//if (_breakCnt > 180) {
		//	_finishedBreakAll = true;

		//	//_breakCnt = 0;

		//	//// ������΂��������w��
		//	//SetBlastDir(VGet(0.0f, 0.0f, 1.0f));

		//	//// �O�Օ\���p�̍��W�������Z�b�g
		//	//for (auto itr = _locus.begin(); itr != _locus.end(); ++itr) {
		//	//	itr->clear();
		//	//}

		//	//// �p�[�c�������ʒu�ɖ߂�
		//	//for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		//	//	MV1ResetFrameUserLocalMatrix(_modelHandle, itr->frameIndex);

		//	//	MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, itr->frameIndex);
		//	//	VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
		//	//	_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(v);
		//	//}
		//}


	}
	// �O�Օ\����On/Off�؂�ւ�
	auto input = XInput::GetInstance();
	if (input->GetTrg(XINPUT_BUTTON_BACK) != 0) {
		_isDrawLocus = !_isDrawLocus;
	}
}

void BreakObject::Render()
{

}

void BreakObject::Activate(bool activate, VECTOR _blastDir)
{
	if (!activate) {
		ResetFrameMatrix();
	}else if (!_isActive) { // ���݂��L����ԂłȂ��A�V�����L��������ꍇ�ɂ̂ݐ�����΂��̐ݒ���s��	
		SetBlastDir(_blastDir);
	}
	_isActive = activate;
}

// ������΂��̕������Z�b�g����
// ����
// vDir : �ӂ���΂��̒��S����
void BreakObject::SetBlastDir(VECTOR vDir)
{
	_modelRotation = MV1GetRotationXYZ(_modelHandle);


	vDir.y = 0.0f;
	vDir = VNorm(vDir);

	// �ӂ���΂����������f���̉�]�l�ɍ��킹�ă��[�J�����W�n�ɕϊ�����
	MATRIX mToLocal = MGetRotY(-_modelRotation.y);
	VECTOR vLocalDir = VTransform(vDir, mToLocal);

	// �p�[�c���Ƃɐ�����΂�����������vDir���� �}maxRange�x�̊ԂŃ����_���Ɍ��肷��
	const int maxRange = 35;
	// �����E���������ɂ�����ő呬�x
	const int maxHorizontalVelocity = 80;
	const int maxVerticalVelocity = 50;

	// �p�[�c���Ƃɂӂ���΂��������Z�b�g����


	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		// ��������
		float angle = rand() % (maxRange * 2);
		angle -= maxRange;
		

		(*itr)->horizontalDir = VTransform(vLocalDir, MGetRotY(Math::DegToRad(angle)));
		(*itr)->horizontalVelocity = rand() % maxHorizontalVelocity;

		// ��������
		(*itr)->verticalVelocity = (rand() % maxVerticalVelocity) + 20.0f;

		// ��]�l
		int deltaRot = 4;

		float angleX = rand() % deltaRot;
		angleX = Math::DegToRad(angleX);
		int mul = rand() % 2 == 0 ? 1 : -1;
		angleX *= mul;

		float angleY = rand() % deltaRot;
		angleY = Math::DegToRad(angleY);
		mul = rand() % 2 == 0 ? 1 : -1;
		angleY *= mul;

		float angleZ = rand() % deltaRot;
		angleZ = Math::DegToRad(angleZ);
		mul = rand() % 2 == 0 ? 1 : -1;
		angleZ *= mul;

		(*itr)->vRot = VGet(angleX, angleY, angleZ);
	}
	//_blastPower = 20.0f;
	_blastDir = vDir;
}

void BreakObject::ResetFrameMatrix()
{
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MV1ResetFrameUserLocalMatrix(_modelHandle, (*itr)->frameIndex);
	}
}

void BreakObject::DrawDebugInfo()
{
	if (_isActive && _isDrawLocus) {
		// �ӂ���΂������̒��S
		{
			VECTOR startPos = MV1GetPosition(_modelHandle);
			DrawLine3D(startPos, VAdd(startPos, VScale(_blastDir, 1000.0f)), GetColor(0, 255, 0));
		}

		// �p�[�c���Ƃɐ�����т̋O�Ղ�\������
		for (auto i = _locus.begin(); i != _locus.end(); ++i) {
			for (auto j = i->begin(); j != i->end() - 1; ++j) {
				DrawLine3D((*j), (*(j + 1)), GetColor(255, 255, 0));
			}
		}
	}
}
