#include "House.h"

std::vector<int>House:: _breakFrame;

House::House() :BuildingBase::BuildingBase() {
	vDiffToCenter = VGet(0.0f, 0.0f, 0.0f);
}

House::~House()
{
}

bool House::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale)
{
	if (!base::Init(modelHandle, startPos, rotation, scale)){ return false;}

	// �����蔻��̐ݒ�
	vDiffToCenter = VGet(0.0f, -250.0f, 0.0f);
	obb.pos = VSub(_pos, vDiffToCenter);
	obb.length[0] = 500.0f;
	obb.length[1] = 500.0f;
	obb.length[2] = 500.0f;

	//// _breakFrame����̏ꍇ�͏�����
	//if (_breakFrame.empty()) {
	//	for (int i = 0; i < MV1GetFrameNum(_modelHandle); i++) {

	//		// �t���[���̖��O���擾����
	//		std::string frameName = MV1GetFrameName(_modelHandle, i);
	//		std::string checkName = "BRK";
	//		if (frameName.substr(0, 3) != checkName) {
	//			continue;
	//		}

	//		_breakFrame.push_back(i);

	//		// �t���[���̍��W�ϊ��s����擾����
	//		MATRIX mFrameLocal = MV1GetFrameLocalMatrix(_modelHandle, i);
	//		// �t���[���̃��[�J�����W�����߂�
	//		VECTOR vFrameLocalPos = VGet(0.0f, 0.0f, 0.0f);
	//		vFrameLocalPos = VTransform(vFrameLocalPos, mFrameLocal);

	//		// ���f���̋N�_���W���猩���t���[���̕������擾����
	//		VECTOR vFrameLocalDir = VGet(0.0f, 0.0f, -1.0f);
	//		if (VSquareSize(vFrameLocalPos) > 0.0000f) {
	//			vFrameLocalDir = VNorm(vFrameLocalPos);
	//			// ������ԕ�����z���}�C�i�X�����Ɍ��肷��
	//			if (vFrameLocalDir.z > 0.0f) {
	//				vFrameLocalDir.z *= -1;
	//			}
	//		}

	//		//FRAME_INFO f = { i, vFrameLocalDir };
	//		//_frameInfo.push_back(f);

	//		//_locus.resize(_locus.size() + 1);
	//	}
	//}

	return true;
}

bool House::Process()
{
	if(!base::Process()) { return false; }
	return true;
}

bool House::Render()
{
	if (!base::Render()) { return false; }	
	return true;
}

bool House::DrawDebugInfo()
{
	if (!base::DrawDebugInfo()) { return false; }
	if (_useCollision) {
		obb.Render(GetColor(255, 255, 255));
	}
	_breakObj->DrawDebugInfo();
	return true;
}
