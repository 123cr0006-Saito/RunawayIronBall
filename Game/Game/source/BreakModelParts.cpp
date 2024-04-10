//----------------------------------------------------------------------
// @filename BreakModelParts.cpp
// ��date: 2024/04/01
// ��author: Morozumi Hiroya
// @explanation
// �������f�����p�[�c���Ƃɐ�����΂��������s���N���X
// ���炩���߃��f���̃p�[�c�𕪊����Ă����A���̃p�[�c�����𗘗p���Đ�����΂��������s��
// �p�[�c�̖��O�̐擪�ɁuBRK�v�����Ă�����̂𐁂���΂��ΏۂƂ��A����ȊO�̃p�[�c�͂��̏�ɂƂǂ܂� 
//----------------------------------------------------------------------
#include "BreakModelParts.h"

BreakModelParts::BreakModelParts()
{
	_isActive = false;
	_finishedBreakAll = false;

	_modelHandle = -1;
	_modelRotation = VGet(0.0f, 0.0f, 0.0f);

	_blastDir = VGet(0.0f, 0.0f, -1.0f);

	_isDrawLocus = false;
}

BreakModelParts::~BreakModelParts()
{
	for (auto&& frame : _frameInfo) {
		delete frame; frame = nullptr;
	}
}

void BreakModelParts::Init(int modelHandle)
{
	_modelHandle = modelHandle;
	for (int i = 0; i < MV1GetFrameNum(_modelHandle); i++) {

		// �t���[���̖��O���擾����
		std::string frameName = MV1GetFrameName(_modelHandle, i);
		std::string checkName = "BRK";
		// �t���[�������uBRK�v�Ŏn�܂�Ȃ��ꍇ��continue
		if (frameName.substr(0, 3) != checkName) {
			continue;
		}

		FRAME_INFO* f = NEW FRAME_INFO();
		f->frameIndex = i;
		f->finishedBreak = false;
		f->horizontalDir = VGet(0.0f, 0.0f, -1.0f);
		f->horizontalVelocity = 0.0f;
		f->verticalVelocity = 0.0f;
		f->vRot = VGet(0.0f, 0.0f, 0.0f);

		_frameInfo.push_back(f);
	}

	// �f�o�b�O�p
	_locus.resize(_frameInfo.size());
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, (*itr)->frameIndex);

		VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
		_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(v);
	}
}

// �X�V����
void BreakModelParts::Process()
{
	// ������΂�����
	// �L����Ԃ��A������΂��������I�����Ă��Ȃ��p�[�c������ꍇ
	if (!_finishedBreakAll && _isActive) {
		_finishedBreakAll = true;
		// �j�Ђ���юU�鏈��
		for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
			// ���łɐ�����΂��������I�����Ă���ꍇ�̓X�L�b�v
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


			// �p�[�c������΂������̏I������
			MATRIX mPivot = MV1GetFrameLocalWorldMatrix(_modelHandle, (*itr)->frameIndex);
			VECTOR pivotPos = VTransform(VGet(0.0f, 0.0f, 0.0f), mPivot);
			// �p�[�c���Ƃ̃s�{�b�g���W���n�ʂ�艺�ɂ���ꍇ�A������΂��������I������
			if (pivotPos.y < 0.0f) {
				(*itr)->finishedBreak = true;
			}

			// �d�͉���
			(*itr)->verticalVelocity -= 2.f;



			// �O�Օ\���p�̍��W��ێ�
			_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(pivotPos);
		}
	}

	// �O�Օ\����On/Off�؂�ւ�
	auto input = XInput::GetInstance();
	if (input->GetTrg(XINPUT_BUTTON_BACK) != 0) {
		_isDrawLocus = !_isDrawLocus;
	}
}

// ������я����̗L�����ݒ�
// @param activate : ������я�����L�������邩�ǂ���
// @param _blastDir : ������΂��̕���
void BreakModelParts::Activate(bool activate, VECTOR blastDir)
{
	if (!activate) {
		ResetFrameMatrix();
	}else if (!_isActive) { // ���݂��L����ԂłȂ��A�V�����L��������ꍇ�ɂ̂ݐ�����΂��̐ݒ���s��	
		SetBlastDir(blastDir);
	}
	_isActive = activate;
}

// ������΂��̕������Z�b�g����
// �p�[�c���ƂɁA������΂��̕�����1�t���[��������̉�]�n��ݒ肷��
// @param vDir : ������΂��̒��S����
void BreakModelParts::SetBlastDir(VECTOR vDir)
{
	_modelRotation = MV1GetRotationXYZ(_modelHandle);

	// XZ���ʏ�ŕ������w�肷��
	vDir.y = 0.0f;
	vDir = VNorm(vDir);

	// �ӂ���΂����������f���̉�]�l�ɍ��킹�ă��[�J�����W�n�ɕϊ�����
	MATRIX mToLocal = MGetRotY(-_modelRotation.y);
	VECTOR vLocalDir = VTransform(vDir, mToLocal);

	// ������΂��̐��������p�[�c���ƂɁAvLocalDir����}maxRange�x�̊ԂŃ����_���Ɍ��肷��
	const int maxRange = 35;
	// �����E���������ɂ�����ő呬�x
	const int maxHorizontalVelocity = 80;
	const int maxVerticalVelocity = 50;

	// �p�[�c���Ƃɂӂ���΂������Ƒ��x���Z�b�g����
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		// ------------------------------
		// ���������̐ݒ�

		// vLocalDir������͈͓��Ń����_���ɕ��������肷��
		float angle = rand() % (maxRange * 2);
		angle -= maxRange;
		// ����
		(*itr)->horizontalDir = VTransform(vLocalDir, MGetRotY(Math::DegToRad(angle)));
		// ���x
		(*itr)->horizontalVelocity = rand() % maxHorizontalVelocity;
		(*itr)->horizontalVelocity = Math::Clamp(20.0f, maxHorizontalVelocity, (*itr)->horizontalVelocity);
		// ------------------------------

		// ------------------------------
		// ���������̐ݒ�

		// ���x
		(*itr)->verticalVelocity = rand() % maxVerticalVelocity;
		(*itr)->verticalVelocity = Math::Clamp(20.0f, maxVerticalVelocity, (*itr)->verticalVelocity);
		// ------------------------------

		// ------------------------------
		// 1�t���[��������̉�]�l�������ƂɃ����_���Ɍ��肷��

		// ��]�l�̍ő�
		int deltaRotMax = 4;

		// X���̉�]�l
		float angleX = rand() % deltaRotMax;
		angleX = Math::DegToRad(angleX);
		// ��]�����������_���Ɍ���
		int mul = rand() % 2 == 0 ? 1 : -1;
		angleX *= mul;

		// Y���̉�]�l
		float angleY = rand() % deltaRotMax;
		angleY = Math::DegToRad(angleY);
		// ��]�����������_���Ɍ���
		mul = rand() % 2 == 0 ? 1 : -1;
		angleY *= mul;

		// Z���̉�]�l
		float angleZ = rand() % deltaRotMax;
		angleZ = Math::DegToRad(angleZ);
		// ��]�����������_���Ɍ���
		mul = rand() % 2 == 0 ? 1 : -1;
		angleZ *= mul;

		// 1�t���[��������̉�]�l���Z�b�g
		(*itr)->vRot = VGet(angleX, angleY, angleZ);
		// ------------------------------
	}
	_blastDir = vDir;
}

// �p�[�c���ƂɓK�������s������Z�b�g���A���f����������Ԃɖ߂�
void BreakModelParts::ResetFrameMatrix()
{
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MV1ResetFrameUserLocalMatrix(_modelHandle, (*itr)->frameIndex);
	}
}

// �f�o�b�O���̕\��
void BreakModelParts::DrawDebugInfo()
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
