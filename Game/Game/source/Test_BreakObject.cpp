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
	}






	// �f�o�b�O�p
	
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, itr->frameIndex);

		VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
		itr->pos = itr->startPos = v;
	}
	
	// ������΂��������w��
	SetBlastDir(VGet(1.0f, 0.0f, 0.0f));
}

void BreakObject::Process()
{
	// �j�Ђ���юU�鏈��
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MATRIX mBefor = MV1GetFrameLocalMatrix(_modelHandle, itr->frameIndex);


		MATRIX mRot = MGetRotVec2(VGet(0.0f, 0.0f, -1.0f), _blastDir);
		//MATRIX mRot = MGetRotY(-(DX_PI / 4.0f));
		VECTOR vDir = VTransform(itr->dir, mRot);

		MATRIX mTrans = MGetTranslate(VScale(vDir, _blastPower));
		MV1SetFrameUserLocalMatrix(_modelHandle, itr->frameIndex, MMult(mBefor, mTrans));


		MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, itr->frameIndex);
		VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
		itr->pos = v;
	}
	//_blastPower -= 0.5f;

	_breakCnt++;

	 // ���Z�b�g
	if (_breakCnt > 90) {
		_breakCnt = 0;
		for (int i = 0; i < MV1GetFrameNum(_modelHandle); i++) {
			MV1ResetFrameUserLocalMatrix(_modelHandle, i);
		}
	}
}

void BreakObject::Render()
{
	MV1SetPosition(_modelHandle, _pos);
	MV1DrawModel(_modelHandle);
}

void BreakObject::SetBlastDir(VECTOR vDir)
{
	//MATRIX mRot = MGetRotVec2(VGet(0.0f, 0.0f, -1.0f), vDir);
	//MATRIX mRot = MGetRotY(-(DX_PI / 4.0f));
	//for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
	//	itr->dir = VTransform(itr->dir, mRot);
	//}

	_blastDir = vDir;
}

void BreakObject::DrawDebugInfo()
{
	// �j�Ђ̐�����т̋O�Ղ�����ŕ\������
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		DrawLine3D(itr->startPos, itr->pos, GetColor(255, 255, 0));
	}

}
