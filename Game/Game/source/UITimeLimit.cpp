//----------------------------------------------------------------------
// @filename UITimeLimit.cpp
// ��date: 2024/03/11
// ��author: saito ko
// @explanation
// �c�莞�Ԃ�\������UI�N���X
//----------------------------------------------------------------------
#include "UITimeLimit.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �ʒu
// ��return ����
//----------------------------------------------------------------------
UITimeLimit::UITimeLimit(VECTOR pos) :UIBase(pos){
	_timeLimit = TimeLimit::GetInstance();
	_numPos = VAdd(pos,VGet(200,130,0));

	_handle = NEW int[10];
	ResourceServer::LoadMultGraph("TimeNum","res/UI/Time/Ui_Time",".png",10,_handle);
	_colonHandle = ResourceServer::Load("TimeColon", "res/UI/Time/UI_Time_Colon.png");
	_frameHandle = ResourceServer::Load("TimeFrame", "res/UI/Time/UI_TIME_Gauge.png");
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// ��return ����
//----------------------------------------------------------------------
UITimeLimit::~UITimeLimit(){
	_timeLimit = nullptr;
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};
//----------------------------------------------------------------------
// @brief �X�V����
// ��return �������Ă��邩
//----------------------------------------------------------------------
bool UITimeLimit::Process(){
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// ��return �������Ă��邩
//----------------------------------------------------------------------
bool UITimeLimit::Draw() {
	DrawGraph(_pos.x, _pos.y, _frameHandle, true);
	int time = _timeLimit->GetTimeLimit();
	int loopCount = 0;

	int x = (int)_numPos.x;
	while (1) {

		int handleX, handleY;
		if (loopCount == 2) {
			// �R������`��
			GetGraphSize(_colonHandle, &handleX, &handleY);
			DrawGraph(x+ handleX, _numPos.y , _colonHandle, true);
			x -= handleX+20;// �����̊Ԋu
		}
		
		// �b����`��
		int num = time % 10;
		DrawGraph(x, _numPos.y, _handle[num], true);
		time /= 10;

		GetGraphSize(_handle[num], &handleX, &handleY);

		x -= handleX+10;// �����̊Ԋu
		loopCount++;// �����ڂ�������

		if (time == 0) {
			break;
		}

	}
	return true;
};