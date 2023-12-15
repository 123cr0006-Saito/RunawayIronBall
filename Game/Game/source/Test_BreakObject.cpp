#include "Test_BreakObject.h"

BreakObject::BreakObject()
{
	_modelHandle = MV1LoadModel("res/Test_BreakObject/centerpivot_2.mv1");
	_pos = VGet(0, 0, 0);
	MV1SetPosition(_modelHandle, _pos);
	//MV1SetScale(_modelHandle, VGet(10.0f, 10.0f, 10.0f));

	//_breakFrameIndex = 35;

	_breakCnt = 0;
}

BreakObject::~BreakObject()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
	}
}

void BreakObject::Process()
{
	for (int i = 0; i < MV1GetFrameNum(_modelHandle); i++) {
		//MV1ResetFrameUserLocalMatrix(_modelHandle, i);
		MATRIX mOrigin = MGetIdent();
		mOrigin = MV1GetFrameLocalMatrix(_modelHandle, i);

		VECTOR vOrigin = VGet(0.0f, 0.0f, 0.0f);
		VECTOR vFramePos = VGet(0.0f, 0.0f, 0.0f);
		vFramePos = VTransform(vFramePos, MV1GetFrameLocalMatrix(_modelHandle, i));
		//vFramePos.z = fabs(vFramePos.z);

		VECTOR vDir = VSub(vFramePos, vOrigin);
		vDir = VNorm(vDir);
		vDir = VScale(vDir, _breakCnt * 10.0f);
		MATRIX mTrans = MGetTranslate(vDir);

		MV1SetFrameUserLocalMatrix(_modelHandle, i, MMult(mOrigin, mTrans));
	}
	//MV1SetMatrix(_modelHandle, mRot);
	_breakCnt++;

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
