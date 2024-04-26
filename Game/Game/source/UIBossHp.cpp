//----------------------------------------------------------------------
// @filename UIBossHp.cpp
// @date: 2024/03/21
// @author: saito ko
// @explanation
// �{�X��HP��\������UI�N���X
//----------------------------------------------------------------------
#include "UIBossHp.h"

const unsigned short UIBossHp::vertex[6]{ 0,1,2,2,1,3 };
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �ʒu
// @return ����
//----------------------------------------------------------------------
UIBossHp::UIBossHp(VECTOR pos) : UIBase(pos){
	_handle = NEW int[2];
	std::string path = "res/UI/Boss/";
	std::string name[2] = { "UI_Boss_Hp_Frame","gauge" };
	for (int i = 0; i < 2; i++) {
		_handle[i] = ResourceServer::Load(name[i], path + name[i] + ".png");
	}

	_ratio = 0.0f;
	_nowRatio = 0.0f;
	_nextRatio = 0.0f;

	// ���ɂ���t���[������Q�[�W����鍷��
	int flontHandleSizeX = 25;
	int flontHandleSizeY = 165;

	GetGraphSize(_handle[1], &flontHandleX, &flontHandleY);

	float posTbl[][4] = {
		{flontHandleSizeX,flontHandleSizeY,0.0f,0.0f},
		{flontHandleSizeX + flontHandleX,flontHandleSizeY,1.0f,0.0f},
		{flontHandleSizeX,flontHandleSizeY + flontHandleY,0.0f,1.0f},
		{flontHandleSizeX + flontHandleX,flontHandleSizeY + flontHandleY,1.0f,1.0f}
	};
	// ���_�̐ݒ�
	for (int i = 0; i < 4; i++) {
		_vertex[i].pos = VAdd(_pos, VGet(posTbl[i][0], posTbl[i][1], 0));
		_vertex[i].u = posTbl[i][2];
		_vertex[i].v = posTbl[i][3];
		_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		_vertex[i].rhw = 1.0f;
	}
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
UIBossHp::~UIBossHp(){
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};
//----------------------------------------------------------------------
// @brief boss�̍ő�HP���猩�����݂�HP�̊������v�Z
// @param nowHp ���݂�HP
// @param maxHp �ő�HP
// @return ����
//----------------------------------------------------------------------
void UIBossHp::SetRatio(int nowHp,int maxHp){
	static int oldHp;
	
	int easingTime = 60.0f / 60.0f * 1000;
	int nowTime = GetNowCount() - _currentTime;

	if (oldHp != nowHp) {
		_nextRatio = (float)nowHp / maxHp;
		_nowRatio = _ratio;
		_currentTime = GetNowCount();
	}

	if (nowTime <= easingTime) {
		_ratio = Easing::OutSine(nowTime, _nowRatio, _nextRatio, easingTime);
		if(_ratio < 0){
			_ratio = 0;
		}
	}

	oldHp = nowHp;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @param nowHp boss�̌��݂�HP
// @param maxHp boss�̍ő�HP
// @return �������Ă��邩
//----------------------------------------------------------------------
bool UIBossHp::Process(int nowHp, int maxHp){
	// HP�̊������v�Z
	SetRatio(nowHp,maxHp);
	// ���_�̐ݒ�
	for (int i = 0; i < 2; i++) {
		_vertex[i * 2].pos.x = _vertex[1].pos.x - flontHandleX * ( _ratio);
		_vertex[i * 2].u = 1-_ratio;
	}

	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �������Ă��邩
//----------------------------------------------------------------------
bool UIBossHp::Draw(){
	// �o���l�t���[��
	DrawGraph(_pos.x, _pos.y, _handle[0], true);

	// �o���l�o�[
	DrawPrimitiveIndexed2D(_vertex, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[1], true);

	return true;
};