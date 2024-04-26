//----------------------------------------------------------------------
// @filename ModeMovie.h
// @date: 2024/03/16
// @author: saito ko
// @explanation
// �Q�[���N���A���ɃG���f�B���O���[�r�[���Đ�����N���X
//----------------------------------------------------------------------
#include "ModeMovie.h"
#include "ModeTitle.h"
#include "ModeFade.h"
//----------------------------------------------------------------------
// @brief ����������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeMovie::Initialize(){
	_movie = ResourceServer::Load("Ending", "res/Movie/Ending.mp4");
	SeekMovieToGraph(_movie, 0);
	PlayMovieToGraph(_movie);
	return true;
};
//----------------------------------------------------------------------
// @brief �I������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeMovie::Terminate(){
	DeleteGraph(_movie);
	return true;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeMovie::Process(){
	// ���[�r�[���I��������^�C�g����ʂɑJ��
	if (!GetMovieStateToGraph(_movie) && !ModeServer::GetInstance()->Search("Fade")) {
		ModeServer::GetInstance()->Del(this);
		ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "Title");
		ModeServer::GetInstance()->Add(NEW ModeFade(2000, true), 10, "Fade");
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeMovie::Render() {
	DrawGraph(0, 0, _movie, true);
	return true;
};